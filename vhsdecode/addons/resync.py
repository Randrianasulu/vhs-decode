from collections import namedtuple
from vhsdecode.addons.vsyncserration import VsyncSerration
import numpy as np
import vhsdecode.utils as utils
import itertools
from lddecode.utils import inrange
import lddecode.core as ldd
import math
import hashlib
from numba import njit


def iretohz(sysparams, ire):
    return sysparams.ire0 + (sysparams.hz_ire * ire)


def hztoire(sysparams, hz, ire0=None):
    if not ire0:
        ire0 = sysparams.ire0
    return (hz - ire0) / sysparams.hz_ire


@njit(cache=True, parallel=True, nogil=True)
def check_levels(data, old_sync, new_sync, new_blank, vsync_hz_ref, hz_ire, full=True):
    """Check if adjusted levels are somewhat sane."""
    # ldd.logger.info("am below new blank %s , amount below half_sync %s", amount_below, amount_below_half_sync)
    # ldd.logger.info("change %s _ %s", old_sync - new_sync, (hz_ire * 15))

    blank_sync_ire_diff = (new_blank - new_sync) / hz_ire

    # ldd.logger.info("ire diff: %s", blank_sync_ire_diff)

    # Check if too far below format's standard sync, or the difference between sync and blank is too large
    # to make sense
    if (vsync_hz_ref - new_sync) > (hz_ire * 15) or blank_sync_ire_diff > 47:
        return False
    if new_sync - old_sync < (hz_ire * 5):
        # Small change - probably ok
        return True

    if full:
        amount_below = len(np.argwhere(data < new_sync)) / len(data)
        amount_below_half_sync = len(np.argwhere(data < new_blank)) / len(data)

        # If there is a lot of data below the detected vsync level, or almost no data below the detected
        # 50% of hsync level it's likely the levels are not correct, so avoid adjusting.
        if amount_below > 0.07 or amount_below_half_sync < 0.005:
            return False

    return True


# search for black level on back porch
def _pulses_blacklevel(demod_05, freq_mhz: float, pulses, vsync_locs, synclevel):

    if not vsync_locs or len(vsync_locs) == 0:
        return None

    # take the eq pulses before and after vsync

    # We skip shorter pulses in case
    before_first = vsync_locs[0]
    after_last = vsync_locs[-1]
    last_index = len(pulses) - 1

    if len(vsync_locs) != 12:
        # Skip pulses that are way to close together to be vsync to avoid assuming noise are pulses.
        # TODO: use linelen and system for num pulses
        while (
            before_first > 1
            and pulses[before_first].start - pulses[before_first - 1].start < 600
        ):
            before_first -= 1

        while (
            after_last < last_index
            and pulses[after_last].start - pulses[after_last + 1].start < 600
        ):
            after_last += 1

    # TODO: This needs to be reworked for samples where the levels vary throughout the field
    r1 = range(max(before_first - 5, 1), before_first) if before_first > 1 else (0, 0)
    r2 = (
        range(after_last + 1, max(after_last + 6, last_index))
        if after_last < last_index - 1
        else (0, 0)
    )

    black_means = []

    # freq_mhz = field.rf.freq

    for i in itertools.chain(r1, r2):
        if i < 0 or i >= len(pulses):
            continue

        p = pulses[i]
        if inrange(p.len, freq_mhz * 0.75, freq_mhz * 3):
            mean_value = np.mean(
                demod_05[int(p.start + (freq_mhz * 5)) : int(p.start + (freq_mhz * 20))]
            )
            black_means.append(mean_value)

    return black_means


"""Pulse definition for findpulses_n. Needs to be outside the function to work with numba.
Make sure to refresh numba cache if modified.
"""
Pulse = namedtuple("Pulse", "start len")


@njit(cache=True, nogil=True, parallel=True)
def _findpulses_numba_raw(sync_ref, high, min_synclen, max_synclen):
    """Locate possible pulses by looking at areas within some range.
    Outputs arrays of starts and lengths
    """
    mid_sync = high
    where_all_picture = np.where(sync_ref > mid_sync)[0]
    locs_len = np.diff(where_all_picture)
    # min_synclen = self.eq_pulselen * 1 / 8
    # max_synclen = self.linelen * 5 / 8
    is_sync = np.bitwise_and(locs_len > min_synclen, locs_len < max_synclen)
    where_all_syncs = np.where(is_sync)[0]
    pulses_lengths = locs_len[where_all_syncs]
    pulses_starts = where_all_picture[where_all_syncs]
    return pulses_starts, pulses_lengths


def _to_pulses_list(pulses_starts, pulses_lengths):
    """Make list of Pulse objects from arrays of pulses starts and lengths"""
    # Not using numba for this right now as it seemed to cause random segfault in tests.
    # list(map(Pulse, pulses_starts, pulses_lengths))
    return [Pulse(z[0], z[1]) for z in zip(pulses_starts, pulses_lengths)]


def _findpulses_numba(sync_ref, high, min_synclen, max_synclen):
    """Locate possible pulses by looking at areas within some range.
    .outputs a list of Pulse tuples
    """
    pulses_starts, pulses_lengths = _findpulses_numba_raw(
        sync_ref, high, min_synclen, max_synclen
    )
    return _to_pulses_list(pulses_starts, pulses_lengths)


def _fallback_vsync_loc_means(
    demod_05, pulses, sample_freq_mhz: float, min_len: int, max_len: int
):
    """Get the mean value of the video level inside pulses above a set threshold.

    Args:
        demod_05 ([type]): Video data to get levels from.
        pulses ([type]): List of detected pulses
        sample_freq_mhz (float): Sample frequency of the data in mhz
        len_threshold (int): only use pulses longer than this threshold.

    Returns:
        [type]: a list of vsync locations in the list of pulses and a list of mean values
    """
    vsync_locs = []
    vsync_means = []

    mean_pos_offset = sample_freq_mhz

    for i, p in enumerate(pulses):
        if p.len > min_len and p.len < max_len:
            vsync_locs.append(i)
            vsync_means.append(
                np.mean(
                    demod_05[
                        int(p.start + mean_pos_offset) : int(
                            p.start + p.len - mean_pos_offset
                        )
                    ]
                )
            )

    return vsync_locs, vsync_means


# stores the last valid blacklevel, synclevel and vsynclocs state
# preliminary solution to fix spurious decoding halts (numpy error case)
class FieldState:
    def __init__(self, sysparams):
        fv = sysparams["FPS"] * 2
        ma_depth = round(fv / 5) if fv < 60 else round(fv / 6)
        # ma_min_watermark = int(ma_depth / 2)
        # TODO: Set to 0 for now to start using detected levels on the first field
        # May want to alter later to do this more dynamically.
        ma_min_watermark = 0
        self._blanklevels = utils.StackableMA(
            window_average=ma_depth, min_watermark=ma_min_watermark
        )
        self._synclevels = utils.StackableMA(
            window_average=ma_depth, min_watermark=ma_min_watermark
        )
        self._locs = None

    def setSyncLevel(self, level):
        self._synclevels.push(level)

    def setLevels(self, sync, blank):
        self._blanklevels.push(blank)
        self.setSyncLevel(sync)

    def getSyncLevel(self):
        return self._synclevels.pull()

    def getLevels(self):
        blevels = self._blanklevels.pull()
        if blevels is not None:
            return self.getSyncLevel(), blevels
        else:
            return None, None

    def setLocs(self, locs):
        self._locs = locs

    def getLocs(self):
        return self._locs

    def hasLevels(self):
        return self._blanklevels.has_values() and self._synclevels.has_values()


class Resync:
    def __init__(self, fs, sysparams, divisor=1, debug=False):

        self.divisor = divisor
        self.debug = debug
        self.samp_rate = fs
        if debug:
            self.SysParams = sysparams.copy()
        self.VsyncSerration = VsyncSerration(fs, sysparams, divisor)
        self.FieldState = FieldState(sysparams)
        self.eq_pulselen = self.VsyncSerration.getEQpulselen()
        self.linelen = self.VsyncSerration.getLinelen()
        self.use_serration = True

        # self._temp_c = 0

    def _debug_field(self, sync_reference):
        ldd.logger.debug(
            "Hashed field sync reference %s"
            % hashlib.md5(sync_reference.tobytes("C")).hexdigest()
        )

    def _pulses_blacklevel(self, field, pulses, vsync_locs, synclevel):
        return _pulses_blacklevel(
            field.data["video"]["demod_05"],
            field.rf.freq,
            pulses,
            vsync_locs,
            synclevel,
        )

    # checks for SysParams consistency
    def _sysparams_consistency_checks(self, field):
        reclamp_ire0 = False
        # AGC is allowed to change two sysparams
        if field.rf.useAGC:
            if not self.debug:
                ldd.logger.warning("Not doing consistency checks with debug disabled!")
                return False
            if field.rf.SysParams["ire0"] != self.SysParams["ire0"]:
                ldd.logger.debug(
                    "AGC changed SysParams[ire0]: %.02f Hz", field.rf.SysParams["ire0"]
                )
                self.SysParams["ire0"] = field.rf.SysParams["ire0"].copy()
                reclamp_ire0 = True

            if field.rf.SysParams["hz_ire"] != self.SysParams["hz_ire"]:
                ldd.logger.debug(
                    "AGC changed SysParams[hz_ire]: %.02f Hz",
                    field.rf.SysParams["hz_ire"],
                )
                self.SysParams["hz_ire"] = field.rf.SysParams["hz_ire"].copy()

        # if self.SysParams != field.rf.SysParams:
        #     ldd.logger.error("SysParams changed during runtime!")
        #     ldd.logger.debug("Original: %s" % self.SysParams)
        #     ldd.logger.debug("Altered : %s" % field.rf.SysParams)
        #     assert False, "SysParams changed during runtime!"

        return reclamp_ire0

    # search for sync and blanking levels from back porch
    def pulses_levels(
        self, field, sp, pulses, pulse_level=0, store_in_field_state=False
    ):

        vsync_len_px = field.usectoinpx(sp.vsync_pulse_us)
        min_len = vsync_len_px * 0.8
        max_len = vsync_len_px * 1.2

        vsync_locs, vsync_means = _fallback_vsync_loc_means(
            field.data["video"]["demod_05"], pulses, field.rf.freq, min_len, max_len
        )

        if len(vsync_means) == 0:
            synclevel = self.FieldState.getSyncLevel()
            if synclevel is None:
                return None, None
        else:
            synclevel = np.median(vsync_means)
            self.FieldState.setSyncLevel(synclevel)
            self.FieldState.setLocs(vsync_locs)

        # TODO: Think this was a bug - need to use absolute locs here,
        # not position in pulse list
        # if vsync_locs is None or not len(vsync_locs):
        #     vsync_locs = self.FieldState.getLocs()

        # Now compute black level and try again
        black_means = self._pulses_blacklevel(field, pulses, vsync_locs, synclevel)
        # print("black_means", black_means)

        # Set to nan if empty to avoid warning.
        blacklevel = (
            math.nan
            if not black_means or len(black_means) == 0
            else np.median(black_means)
        )
        # If black level is below sync level, something has gone very wrong.
        if blacklevel < synclevel:
            blacklevel = math.nan

        # if store_in_field_state:
        #     import matplotlib.pyplot as plt

        #     fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True)
        #     ax1.plot(field.data["video"]["demod_05"])
        #     ax1.axhline(synclevel)
        #     ax1.axhline(pulse_level, color="#00FF00")
        #     if blacklevel is not math.nan:
        #         ax1.axhline(blacklevel, color="#000000")
        #     for p in pulses:
        #         ax2.axvline(p.start, color="#00FF00")
        #         ax2.axvline(p.start + p.len, color="#0000FF")
        #     for loc in vsync_locs:
        #         ax2.axvline(pulses[loc].start, color="#FF0000")
        #     # ax2.plot(self.Filters["FVideo05"])
        #     plt.show()

        if np.isnan(blacklevel).any() or np.isnan(synclevel).any():
            ldd.logger.debug("blacklevel or synclevel had a NaN!")
            # utils.plot_scope(field.data["video"]["demod_05"], title='Failed field demod05')
            sl, bl = self.FieldState.getLevels()
            if bl is not None and sl is not None:
                blacklevel, synclevel = bl, sl
            else:
                return None, None
        else:
            # Make sure these levels are sane before using them.
            if self.level_check(
                field.rf.sysparams_const,
                synclevel,
                blacklevel,
                field.data["video"]["demod_05"],
                False,
            ):
                if store_in_field_state:
                    self.FieldState.setLevels(synclevel, blacklevel)
            else:
                ldd.logger.debug("level check failed in pulses_levels!")
                return None, None

        return synclevel, blacklevel

    def findpulses_range(self, sp, vsync_hz, blank_hz=None):
        if not blank_hz:
            # Fall back to assume blank is at standard 0 ire.
            blank_hz = iretohz(sp, 0)
        sync_ire = hztoire(sp, vsync_hz)
        pulse_hz_min = iretohz(sp, sync_ire - 10)
        ## Look for pulses at the halfway between vsync tip and blanking.
        pulse_hz_max = (iretohz(sp, sync_ire) + blank_hz) / 2
        return pulse_hz_min, pulse_hz_max

        # lddu.findpulses() equivalent

    def findpulses(self, sync_ref, high):
        return _findpulses_numba(
            sync_ref, high, self.eq_pulselen * 1 / 8, self.linelen * 5 / 8
        )

    def _findpulses_arr(self, sync_ref, high):
        return _findpulses_numba_raw(
            sync_ref, high, self.eq_pulselen * 1 / 8, self.linelen * 5 / 8
        )

    def _findpulses_arr_reduced(self, sync_ref, high, divisor):
        """Run findpulses using only every divisor samples"""
        min_len = (self.eq_pulselen * 1 / 8) / divisor
        max_len = (self.linelen * 5 / 8) / divisor

        pulses_starts, pulses_lengths = _findpulses_numba_raw(
            sync_ref[::divisor], high, min_len, max_len
        )

        pulses_starts *= divisor
        pulses_lengths *= divisor

        return pulses_starts, pulses_lengths

    def add_pulselevels_to_serration_measures(self, field, demod_05, sp):
        if self.VsyncSerration.hasSerration():
            sync, blank = self.VsyncSerration.getLevels()
        else:
            # it starts finding the sync from the minima in 5 ire steps
            ire_step = 5
            min_sync = np.min(demod_05)
            retries = 30
            min_vsync_check = field.usectoinpx(sp.vsync_pulse_us) * 0.8
            num_assumed_vsyncs_prev = 0
            prev_min_sync = min_sync
            found_candidate = False
            check_next = True
            while retries > 0:
                pulse_hz_min, pulse_hz_max = self.findpulses_range(sp, min_sync)
                pulses_starts, pulses_lengths = self._findpulses_arr_reduced(
                    demod_05, pulse_hz_max, self.divisor
                )
                # this number might need calculation
                if len(pulses_lengths) > 200:
                    # Check that at least 2 pulses are long enough to be vsync to avoid noise
                    # being counted as pulses
                    num_assumed_vsyncs = len(
                        pulses_lengths[pulses_lengths > min_vsync_check]
                    )
                    if len(pulses_lengths[pulses_lengths > min_vsync_check]) > 4:
                        if num_assumed_vsyncs == 12 and not check_next:
                            break
                        elif (
                            not found_candidate
                            or num_assumed_vsyncs > num_assumed_vsyncs_prev
                        ):
                            found_candidate = True
                            num_assumed_vsyncs_prev = num_assumed_vsyncs
                            prev_min_sync = min_sync
                            check_next = True
                        elif (
                            num_assumed_vsyncs < num_assumed_vsyncs_prev
                            or check_next is False
                        ):
                            # Use previous
                            min_sync = prev_min_sync
                            pulse_hz_min, pulse_hz_max = self.findpulses_range(
                                sp, min_sync
                            )
                            pulses_starts, pulses_lengths = self._findpulses_arr(
                                demod_05, pulse_hz_max
                            )
                            break
                        else:
                            check_next = False

                min_sync = iretohz(sp, hztoire(sp, min_sync) + ire_step)
                retries -= 1

            pulses = _to_pulses_list(pulses_starts, pulses_lengths)

            sync, blank = self.pulses_levels(field, sp, pulses, pulse_hz_max)
            # chewed tape case
            if sync is None or blank is None:
                ldd.logger.debug("Level detection failed - sync or blank is None")
                return

        # the tape chewing test passed, then it should find sync
        pulse_hz_min, pulse_hz_max = self.findpulses_range(sp, sync, blank_hz=blank)
        pulses = self.findpulses(demod_05, pulse_hz_max)

        if False:
            import matplotlib.pyplot as plt

            # ldd.logger.info("hz to ire %s", hztoire(sp, blank))
            fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True)
            ax1.plot(field.data["video"]["demod_05"])
            ax1.axhline(sync)
            ax1.axhline(pulse_hz_max, color="#00FF00")
            # ax1.axhline((sync + blank) / 2.0 , color="#FF0000")
            ax1.axhline(blank, color="#000000")
            for p in pulses:
                ax2.axvline(p.start, color="#00FF00")
                ax2.axvline(p.start + p.len, color="#0000FF")
            # ax2.plot(self.Filters["FVideo05"])
            plt.show()

        f_sync, f_blank = self.pulses_levels(
            field, sp, pulses, pulse_hz_max, store_in_field_state=True
        )
        if f_sync is not None and f_blank is not None:
            self.VsyncSerration.push_levels((f_sync, f_blank))
        else:
            ldd.logger.debug(
                "Level detection had issues, so don't store anything in VsyncSerration."
            )

    # Do a level check
    def level_check(self, sysparams_const, sync, blank, sync_reference, full=True):
        vsync_hz = (
            sysparams_const.vsync_hz
        )  # field.rf.iretohz(field.rf.SysParams["vsync_ire"])
        # TODO: See if we need to read vsync_ire from sysparams here
        return check_levels(
            sync_reference,
            vsync_hz,
            sync,
            blank,
            sysparams_const.vsync_hz,
            sysparams_const.hz_ire,
            full,
        )

    def getpulses_override(self, field):
        if self.use_serration:
            return self.getpulses_serration(field)
        else:
            import vhsdecode.leveldetect

            sync, blank = None, None
            if self.FieldState.hasLevels():
                sync, blank = self.FieldState.getLevels()
                pulses = self.findpulses(
                    field.data["video"]["demod_05"], (blank + sync) / 2
                )
                if len(pulses) > 200 and len(pulses) < 800:
                    return pulses
                ldd.logger.info("Re-checking levels..")

            def_sync = field.rf.iretohz(field.rf.SysParams["vsync_ire"])
            def_blank = field.rf.iretohz(field.rf.SysParams["ire0"])
            sync, blank = vhsdecode.leveldetect.find_sync_levels(
                field.data["video"]["demod_05"],
                def_sync,
                def_blank,
                field.get_linefreq(),
            )
            self.FieldState.setLevels(sync, blank)

            return self.findpulses(field.data["video"]["demod_05"], (blank + sync) / 2)

    def getpulses_serration(self, field):
        """Find sync pulses in the demodulated video signal

        NOTE: TEMPORARY override until an override for the value itself is added upstream.
        """

        sp = field.rf.sysparams_const

        sync_reference = field.data["video"]["demod_05"]
        if self.debug:
            self._debug_field(sync_reference)

        # measures the serration levels if possible
        self.VsyncSerration.work(sync_reference)
        # adds the sync and blanking levels from the back porch
        self.add_pulselevels_to_serration_measures(field, sync_reference, sp)

        # safe clips the bottom of the sync pulses but leaves picture area unchanged
        # NOTE: Disabled for now as it doesn't seem to have much purpose at the moment and can
        # cause weird artifacts on the output.
        demod_data = (
            field.data["video"]["demod"]
            if not field.rf.options.sync_clip
            else self.VsyncSerration.safe_sync_clip(
                sync_reference, field.data["video"]["demod"]
            )
        )

        # if self._temp_c == 1:
        #     np.savetxt("PAL_GOOD.txt.gz", sync_reference)
        # self._temp_c += 1

        # if it has levels, then compensate blanking bias
        if self.VsyncSerration.hasLevels() or self.FieldState.hasLevels():
            if self.VsyncSerration.hasLevels():
                new_sync, new_blank = self.VsyncSerration.getLevels()
                if self.level_check(sp, new_sync, new_blank, sync_reference):
                    sync, blank = new_sync, new_blank
                elif self.FieldState.hasLevels():
                    sync, blank = self.FieldState.getLevels()
                    ldd.logger.debug(
                        "Level check failed on serration measured levels [new_sync: %s, new_blank: %s], falling back to levels from FieldState [sync %s, blank %s].",
                        new_sync,
                        new_blank,
                        sync,
                        blank,
                    )
                else:
                    # Check failed on serration levels and field state does not contain levels
                    # TODO: Handle properly if this occurs
                    ldd.logger.debug(
                        "Level check failed on serration measured levels, using defaults."
                    )

                    sync = sp.ire0
                    blank = sp.vsync_hz
            else:
                sync, blank = self.FieldState.getLevels()

            if self._sysparams_consistency_checks(field):
                field.rf.SysParams["ire0"] = blank

            dc_offset = sp.ire0 - blank
            sync_reference += dc_offset
            if not field.rf.options.disable_dc_offset:
                demod_data += dc_offset
                field.data["video"]["demod"] = demod_data
            sync, blank = sync + dc_offset, blank + dc_offset

            field.data["video"]["demod_05"] = sync_reference
            pulse_hz_min, pulse_hz_max = self.findpulses_range(sp, sync)
        else:
            # pass one using standard levels (fallback sync logic)
            # pulse_hz range:  vsync_ire - 10, maximum is the 50% crossing point to sync
            pulse_hz_min, pulse_hz_max = self.findpulses_range(sp, sp.vsync_hz)

            # checks if the DC offset is abnormal before correcting it
            new_sync = self.VsyncSerration.mean_bias()
            vsync_hz = sp.vsync_hz
            new_blank = iretohz(sp, hztoire(sp, new_sync) / 2)

            check = self.level_check(sp, new_sync, new_blank, sync_reference)
            if (
                not field.rf.options.disable_dc_offset
                and not pulse_hz_min < new_sync < vsync_hz
                and check
            ):
                field.data["video"]["demod_05"] = sync_reference - new_sync + vsync_hz
                field.data["video"]["demod"] = demod_data - new_sync + vsync_hz

        return self.findpulses(field.data["video"]["demod_05"], pulse_hz_max)