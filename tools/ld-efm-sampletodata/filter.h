/************************************************************************

    filter.h

    ld-efm-sampletodata - EFM sample to data processor for ld-decode
    Copyright (C) 2019 Simon Inns

    This file is part of ld-decode-tools.

    ld-efm-sampletodata is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

#ifndef FILTER_H
#define FILTER_H

#include <QCoreApplication>
#include <QDebug>

class Filter
{
public:
    Filter();

    QVector<qint16> dcBlocking(QVector<qint16> x);
    QVector<qint16> channelEqualizer(QVector<qint16> inputSample);

private:
    // Channel equalizer filter ---------------------------------------------------------------------------------------

    // M = Amount of over sampling
    // D = Number of symbols to be spanned by the impulse response
    // N = D * M (where both M and D are integers)
    //
    // Oversampling is 40,000,000 / 4,321,800 = 9.255402841
    // Best if D and M are integer multiples
    //
    // N = 11 * 9 = 99 taps

    // Generated by:   http://www-users.cs.york.ac.uk/~fisher/mkfilter
    //    filtertype 	= 	Raised Cosine
    //    samplerate 	= 	40000000
    //    corner 	= 	4321800
    //    beta 	= 	0.25
    //    impulselen 	= 	99
    //    racos 	= 	sqrt
    //    comp 	= 	no
    //    bits 	=
    //    logmin 	=

    /* Digital filter designed by mkfilter/mkshape/gencode   A.J. Fisher
       Command line: /www/usr/fisher/helpers/mkshape -r 1.0804500000e-01 2.5000000000e-01 99 -l */

    static const qint32 ceNZeros = 98;
    static constexpr qreal ceGain = 4.634844969e+00;
    qreal ceXv[ceNZeros+1];

    const qreal ceXcoeffs[ceNZeros+1] = {
        +0.0021635109, +0.0031505110, +0.0020790449, -0.0004332256,
        -0.0027258909, -0.0031784415, -0.0013550390, +0.0015864963,
        +0.0035763805, +0.0030283407, +0.0000339340, -0.0035102046,
        -0.0050483368, -0.0031180585, +0.0014580765, +0.0058491500,
        +0.0069453533, +0.0035061976, -0.0027253753, -0.0078536927,
        -0.0082739811, -0.0032283518, +0.0043031550, +0.0092313075,
        +0.0077165685, +0.0001379522, -0.0085019461, -0.0114595936,
        -0.0049558781, +0.0083071365, +0.0195720617, +0.0189535112,
        +0.0024298590, -0.0234783535, -0.0431296617, -0.0398593896,
        -0.0070804731, +0.0440733780, +0.0863355787, +0.0885263634,
        +0.0337285037, -0.0657383067, -0.1657355904, -0.2041968956,
        -0.1274291399, +0.0823649547, +0.3917138536, +0.7216020090,
        +0.9739089866, +1.0683122717, +0.9739089866, +0.7216020090,
        +0.3917138536, +0.0823649547, -0.1274291399, -0.2041968956,
        -0.1657355904, -0.0657383067, +0.0337285037, +0.0885263634,
        +0.0863355787, +0.0440733780, -0.0070804731, -0.0398593896,
        -0.0431296617, -0.0234783535, +0.0024298590, +0.0189535112,
        +0.0195720617, +0.0083071365, -0.0049558781, -0.0114595936,
        -0.0085019461, +0.0001379522, +0.0077165685, +0.0092313075,
        +0.0043031550, -0.0032283518, -0.0082739811, -0.0078536927,
        -0.0027253753, +0.0035061976, +0.0069453533, +0.0058491500,
        +0.0014580765, -0.0031180585, -0.0050483368, -0.0035102046,
        +0.0000339340, +0.0030283407, +0.0035763805, +0.0015864963,
        -0.0013550390, -0.0031784415, -0.0027258909, -0.0004332256,
        +0.0020790449, +0.0031505110, +0.0021635109,
    };

    // Method prototypes ----------------------------------------------------------------------------------------------
    qreal channelEqualizerFir(qreal inputSample);
};

#endif // FILTER_H
