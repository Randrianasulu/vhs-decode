#ifndef DEEMP2_H
#define DEEMP2_H

std::vector<double> c2_boost_b = {
    4.387575368399077e-04, 1.632895622862063e-03, 2.537748875966841e-03, 2.044228963833118e-03,
    -1.184428038595139e-03, -6.824725464908456e-03, -1.141112158933924e-02, -9.361696552715496e-03,
    2.990718843871222e-03, 2.256903353665730e-02, 3.778448293341898e-02, 3.256028527533292e-02,
    -4.803881013164025e-03, -7.274845926804281e-02, -1.535119715490024e-01, -2.191957296541930e-01,
    7.556710529176590e-01, -2.191957296541930e-01, -1.535119715490024e-01, -7.274845926804281e-02,
    -4.803881013164026e-03, 3.256028527533292e-02, 3.778448293341900e-02, 2.256903353665731e-02,
    2.990718843871223e-03, -9.361696552715496e-03, -1.141112158933924e-02, -6.824725464908461e-03,
    -1.184428038595139e-03, 2.044228963833118e-03, 2.537748875966844e-03, 1.632895622862065e-03,
    4.387575368399077e-04
};
std::vector<double> c2_boost_a = {
    1.000000000000000e+00
};

Filter f2_boost(c2_boost_b, c2_boost_a);

std::vector<double> c2_boost10_b = {
    6.263255080839853e-04, -3.915507880949285e-04, -1.916735209842007e-03, -3.819172995004063e-03,
    -5.042485617935914e-03, -3.758388089387950e-03, 1.610695988129703e-03, 1.088981776984434e-02,
    2.102516852905989e-02, 2.621377901216246e-02, 1.965771061382473e-02, -3.529284119720893e-03,
    -4.347367318614239e-02, -9.417406914481029e-02, -1.445325546844917e-01, -1.816730616404486e-01,
    8.036072361277077e-01, -1.816730616404486e-01, -1.445325546844917e-01, -9.417406914481029e-02,
    -4.347367318614239e-02, -3.529284119720893e-03, 1.965771061382474e-02, 2.621377901216247e-02,
    2.102516852905990e-02, 1.088981776984434e-02, 1.610695988129703e-03, -3.758388089387953e-03,
    -5.042485617935915e-03, -3.819172995004063e-03, -1.916735209842009e-03, -3.915507880949288e-04,
    6.263255080839853e-04
};
std::vector<double> c2_boost10_a = {
    1.000000000000000e+00
};

Filter f2_boost10(c2_boost10_b, c2_boost10_a);

std::vector<double> c2_color_b = {
    4.296424055924472e-03, 4.820368376227812e-03, 6.300710847429372e-03, 8.706323730295659e-03,
    1.196573459156953e-02, 1.596958998357159e-02, 2.057483967978480e-02, 2.561047195098441e-02,
    3.088455666359826e-02, 3.619229008456978e-02, 4.132468628021605e-02, 4.607752734167055e-02,
    5.026017002027362e-02, 5.370381051353325e-02, 5.626883200069820e-02, 5.785090007668063e-02,
    5.838552760594402e-02, 5.785090007668063e-02, 5.626883200069820e-02, 5.370381051353325e-02,
    5.026017002027363e-02, 4.607752734167055e-02, 4.132468628021606e-02, 3.619229008456978e-02,
    3.088455666359827e-02, 2.561047195098441e-02, 2.057483967978480e-02, 1.596958998357160e-02,
    1.196573459156954e-02, 8.706323730295659e-03, 6.300710847429379e-03, 4.820368376227816e-03,
    4.296424055924472e-03
};
std::vector<double> c2_color_a = {
    1.000000000000000e+00
};

Filter f2_color(c2_color_b, c2_color_a);

std::vector<double> c2_lpf_b = {
    -1.676812318972604e-03, -5.374936889747953e-04, 2.254940908923396e-03, 4.022274254815427e-03,
    -1.073502430983471e-04, -9.045704503701424e-03, -1.052619532644138e-02, 5.739018325719545e-03,
    2.654003770829400e-02, 1.932369268654868e-02, -2.683398362195289e-02, -6.680566651536424e-02,
    -2.689580509531038e-02, 1.159324474560185e-01, 2.868022627927465e-01, 3.636286743614999e-01,
    2.868022627927465e-01, 1.159324474560185e-01, -2.689580509531039e-02, -6.680566651536425e-02,
    -2.683398362195290e-02, 1.932369268654869e-02, 2.654003770829401e-02, 5.739018325719548e-03,
    -1.052619532644138e-02, -9.045704503701424e-03, -1.073502430983470e-04, 4.022274254815428e-03,
    2.254940908923395e-03, -5.374936889747953e-04, -1.676812318972604e-03
};
std::vector<double> c2_lpf_a = {
    1.000000000000000e+00
};

Filter f2_lpf(c2_lpf_b, c2_lpf_a);

std::vector<double> c2_lpf42_b = {
    1.613172645086047e-03, 6.727619992811481e-04, -1.621472972157979e-03, -4.439803146181066e-03,
    -4.386700689000995e-03, 2.049823178361324e-03, 1.272098110625865e-02, 1.733049859690709e-02,
    4.455855663190380e-03, -2.475177666520113e-02, -4.870918525511100e-02, -3.491898896829457e-02,
    3.559709425123384e-02, 1.470689789837513e-01, 2.507784566649503e-01, 2.930806092138532e-01,
    2.507784566649503e-01, 1.470689789837513e-01, 3.559709425123384e-02, -3.491898896829458e-02,
    -4.870918525511102e-02, -2.475177666520114e-02, 4.455855663190384e-03, 1.733049859690709e-02,
    1.272098110625865e-02, 2.049823178361324e-03, -4.386700689000995e-03, -4.439803146181067e-03,
    -1.621472972157978e-03, 6.727619992811481e-04, 1.613172645086047e-03
};
std::vector<double> c2_lpf42_a = {
    1.000000000000000e+00
};

Filter f2_lpf42(c2_lpf42_b, c2_lpf42_a);

std::vector<double> c2_lpf_comb_b = {
    6.577901011847751e-04, 1.160441860403558e-03, 2.089095713318517e-03, 3.693711458291047e-03,
    6.191480178152384e-03, 9.736650443288511e-03, 1.439523966910153e-02, 2.012773832743940e-02,
    2.678212842593536e-02, 3.409846319368048e-02, 4.172500964596507e-02, 4.924467913455562e-02,
    5.620930860945592e-02, 6.217844160063366e-02, 6.675870045103509e-02, 6.963970790673726e-02,
    7.062282656164354e-02, 6.963970790673726e-02, 6.675870045103510e-02, 6.217844160063366e-02,
    5.620930860945592e-02, 4.924467913455562e-02, 4.172500964596510e-02, 3.409846319368049e-02,
    2.678212842593537e-02, 2.012773832743940e-02, 1.439523966910153e-02, 9.736650443288520e-03,
    6.191480178152386e-03, 3.693711458291047e-03, 2.089095713318519e-03, 1.160441860403559e-03,
    6.577901011847751e-04
};
std::vector<double> c2_lpf_comb_a = {
    1.000000000000000e+00
};

Filter f2_lpf_comb(c2_lpf_comb_b, c2_lpf_comb_a);

std::vector<double> c2_lpf4_b = {
    -1.676812318972604e-03, -5.374936889747953e-04, 2.254940908923396e-03, 4.022274254815427e-03,
    -1.073502430983471e-04, -9.045704503701424e-03, -1.052619532644138e-02, 5.739018325719545e-03,
    2.654003770829400e-02, 1.932369268654868e-02, -2.683398362195289e-02, -6.680566651536424e-02,
    -2.689580509531038e-02, 1.159324474560185e-01, 2.868022627927465e-01, 3.636286743614999e-01,
    2.868022627927465e-01, 1.159324474560185e-01, -2.689580509531039e-02, -6.680566651536425e-02,
    -2.683398362195290e-02, 1.932369268654869e-02, 2.654003770829401e-02, 5.739018325719548e-03,
    -1.052619532644138e-02, -9.045704503701424e-03, -1.073502430983470e-04, 4.022274254815428e-03,
    2.254940908923395e-03, -5.374936889747953e-04, -1.676812318972604e-03
};
std::vector<double> c2_lpf4_a = {
    1.000000000000000e+00
};

Filter f2_lpf4(c2_lpf4_b, c2_lpf4_a);

std::vector<double> c2_lpf10_b = {
    1.530960711199010e-03, 4.310659751765299e-04, -1.889456548691172e-03, -4.446167541609468e-03,
    -3.877803004174977e-03, 2.888264273604397e-03, 1.315839165131086e-02, 1.665818472124762e-02,
    2.830990350722843e-03, -2.611421204951105e-02, -4.845515040504186e-02, -3.288557930240953e-02,
    3.796538586827169e-02, 1.478202687871681e-01, 2.491879046693181e-01, 2.903939036868379e-01,
    2.491879046693181e-01, 1.478202687871681e-01, 3.796538586827171e-02, -3.288557930240955e-02,
    -4.845515040504188e-02, -2.611421204951106e-02, 2.830990350722846e-03, 1.665818472124763e-02,
    1.315839165131086e-02, 2.888264273604397e-03, -3.877803004174976e-03, -4.446167541609470e-03,
    -1.889456548691171e-03, 4.310659751765299e-04, 1.530960711199010e-03
};
std::vector<double> c2_lpf10_a = {
    1.000000000000000e+00
};

Filter f2_lpf10(c2_lpf10_b, c2_lpf10_a);

std::vector<double> c2_sync2_b = {
    6.075969065158128e-03, 7.279892021844364e-03, 1.079480817355710e-02, 1.639167595194183e-02,
    2.369650623004010e-02, 3.221586593521700e-02, 4.137077454308704e-02, 5.053664384139357e-02,
    5.908648107002102e-02, 6.643434042255983e-02, 7.207598498652686e-02, 7.562391249497803e-02,
    7.683429052735022e-02, 7.562391249497803e-02, 7.207598498652687e-02, 6.643434042255984e-02,
    5.908648107002104e-02, 5.053664384139357e-02, 4.137077454308704e-02, 3.221586593521702e-02,
    2.369650623004010e-02, 1.639167595194184e-02, 1.079480817355711e-02, 7.279892021844373e-03,
    6.075969065158128e-03
};
std::vector<double> c2_sync2_a = {
    1.000000000000000e+00
};

Filter f2_sync(c2_sync2_b, c2_sync2_a);

std::vector<double> c2_ntscsyncbpf4_b = {
    -2.194588824998180e-03, -2.241885181851026e-03, -3.556938285547979e-08, 7.149468083983168e-03,
    1.504464947108959e-02, 1.413792770224243e-02, 4.827070127607497e-08, -1.910232916198251e-02,
    9.744134904203963e-01, -1.910232916198251e-02, 4.827070127607497e-08, 1.413792770224243e-02,
    1.504464947108960e-02, 7.149468083983167e-03, -3.556938285547979e-08, -2.241885181851028e-03,
    -2.194588824998180e-03
};
std::vector<double> c2_ntscsyncbpf4_a = {
    1.000000000000000e+00
};

Filter f2_ntscsyncbpf4(c2_ntscsyncbpf4_b, c2_ntscsyncbpf4_a);

std::vector<double> c2_esync8_b = {
    -1.288201184857409e-03, 4.093423113188407e-04, 6.224527039199805e-03, 2.110849830652016e-02,
    4.739617605088404e-02, 8.250075794592006e-02, 1.188577010335986e-01, 1.464290909649238e-01,
    1.567242150649841e-01, 1.464290909649239e-01, 1.188577010335986e-01, 8.250075794592010e-02,
    4.739617605088405e-02, 2.110849830652016e-02, 6.224527039199806e-03, 4.093423113188410e-04,
    -1.288201184857409e-03
};
std::vector<double> c2_esync8_a = {
    1.000000000000000e+00
};

Filter f2_esync8(c2_esync8_b, c2_esync8_a);

std::vector<double> c2_esync4_b = {
    -1.288201184857409e-03, 4.093423113188407e-04, 6.224527039199805e-03, 2.110849830652016e-02,
    4.739617605088404e-02, 8.250075794592006e-02, 1.188577010335986e-01, 1.464290909649238e-01,
    1.567242150649841e-01, 1.464290909649239e-01, 1.188577010335986e-01, 8.250075794592010e-02,
    4.739617605088405e-02, 2.110849830652016e-02, 6.224527039199806e-03, 4.093423113188410e-04,
    -1.288201184857409e-03
};
std::vector<double> c2_esync4_a = {
    1.000000000000000e+00
};

Filter f2_esync4(c2_esync4_b, c2_esync4_a);

std::vector<double> c2_esync10_b = {
    1.314948153735854e-03, 4.176184084038102e-03, 1.237701116889664e-02, 2.877664743111265e-02,
    5.353852279529462e-02, 8.340992711770892e-02, 1.123646075634858e-01, 1.334532546868392e-01,
    1.411777939977763e-01, 1.334532546868392e-01, 1.123646075634859e-01, 8.340992711770898e-02,
    5.353852279529463e-02, 2.877664743111264e-02, 1.237701116889665e-02, 4.176184084038107e-03,
    1.314948153735854e-03
};
std::vector<double> c2_esync10_a = {
    1.000000000000000e+00
};

Filter f2_esync10(c2_esync10_b, c2_esync10_a);

std::vector<double> c2_esync32_b = {
    6.251856365887974e-03, 1.007490458916189e-02, 2.067129060378125e-02, 3.783952993949010e-02,
    5.967985542286956e-02, 8.290115477059432e-02, 1.035070332890583e-01, 1.176969186742433e-01,
    1.227549126898265e-01, 1.176969186742433e-01, 1.035070332890583e-01, 8.290115477059436e-02,
    5.967985542286956e-02, 3.783952993949010e-02, 2.067129060378126e-02, 1.007490458916190e-02,
    6.251856365887974e-03
};
std::vector<double> c2_esync32_a = {
    1.000000000000000e+00
};

Filter f2_esync32(c2_esync32_b, c2_esync32_a);

std::vector<double> c2_psync8_b = {
    1.066668471191957e-03, 5.535133088366486e-04, -3.625255884794497e-04, -2.100536905877551e-03,
    -4.826528356082359e-03, -8.144924467056686e-03, -1.093933098527869e-02, -1.146541473780160e-02,
    -7.719669475065309e-03, 1.989775929431125e-03, 1.842709887066232e-02, 4.096211870315181e-02,
    6.742360777056634e-02, 9.436053379417701e-02, 1.176813755427127e-01, 1.335249840845047e-01,
    1.391385080808141e-01, 1.335249840845047e-01, 1.176813755427128e-01, 9.436053379417701e-02,
    6.742360777056636e-02, 4.096211870315181e-02, 1.842709887066233e-02, 1.989775929431125e-03,
    -7.719669475065311e-03, -1.146541473780160e-02, -1.093933098527869e-02, -8.144924467056693e-03,
    -4.826528356082360e-03, -2.100536905877551e-03, -3.625255884794501e-04, 5.535133088366489e-04,
    1.066668471191957e-03
};
std::vector<double> c2_psync8_a = {
    1.000000000000000e+00
};

Filter f2_psync8(c2_psync8_b, c2_psync8_a);

std::vector<double> c2_psync4_b = {
    1.066668471191957e-03, 5.535133088366486e-04, -3.625255884794497e-04, -2.100536905877551e-03,
    -4.826528356082359e-03, -8.144924467056686e-03, -1.093933098527869e-02, -1.146541473780160e-02,
    -7.719669475065309e-03, 1.989775929431125e-03, 1.842709887066232e-02, 4.096211870315181e-02,
    6.742360777056634e-02, 9.436053379417701e-02, 1.176813755427127e-01, 1.335249840845047e-01,
    1.391385080808141e-01, 1.335249840845047e-01, 1.176813755427128e-01, 9.436053379417701e-02,
    6.742360777056636e-02, 4.096211870315181e-02, 1.842709887066233e-02, 1.989775929431125e-03,
    -7.719669475065311e-03, -1.146541473780160e-02, -1.093933098527869e-02, -8.144924467056693e-03,
    -4.826528356082360e-03, -2.100536905877551e-03, -3.625255884794501e-04, 5.535133088366489e-04,
    1.066668471191957e-03
};
std::vector<double> c2_psync4_a = {
    1.000000000000000e+00
};

Filter f2_psync4(c2_psync4_b, c2_psync4_a);

std::vector<double> c2_psync10_b = {
    -9.831262845988054e-04, -1.602938332776909e-03, -2.560383941941154e-03, -3.812673850918667e-03,
    -4.997128108381489e-03, -5.425112160463596e-03, -4.176757161302743e-03, -2.857242735306487e-04,
    7.022104195861240e-03, 1.809009290559837e-02, 3.265201738363843e-02, 4.976326851829847e-02,
    6.787013228839547e-02, 8.501465959593303e-02, 9.914106447730006e-02, 1.084446088182488e-01,
    1.116917918612803e-01, 1.084446088182488e-01, 9.914106447730008e-02, 8.501465959593303e-02,
    6.787013228839549e-02, 4.976326851829847e-02, 3.265201738363845e-02, 1.809009290559837e-02,
    7.022104195861242e-03, -2.857242735306487e-04, -4.176757161302743e-03, -5.425112160463600e-03,
    -4.997128108381490e-03, -3.812673850918667e-03, -2.560383941941157e-03, -1.602938332776910e-03,
    -9.831262845988054e-04
};
std::vector<double> c2_psync10_a = {
    1.000000000000000e+00
};

Filter f2_psync10(c2_psync10_b, c2_psync10_a);

std::vector<double> c2_dsync2_b = {
    4.529500990800284e-03, 5.042565886563957e-03, 6.543687151896889e-03, 8.981720927353037e-03,
    1.226825957164825e-02, 1.628097813268541e-02, 2.086834898291218e-02, 2.585553998651212e-02,
    3.105126295536169e-02, 3.625529990284258e-02, 4.126640635953664e-02, 4.589027496220054e-02,
    4.994723935586487e-02, 5.327940831632746e-02, 5.575694252624885e-02, 5.728322071856454e-02,
    5.779868654536129e-02, 5.728322071856454e-02, 5.575694252624885e-02, 5.327940831632746e-02,
    4.994723935586488e-02, 4.589027496220054e-02, 4.126640635953666e-02, 3.625529990284259e-02,
    3.105126295536169e-02, 2.585553998651212e-02, 2.086834898291218e-02, 1.628097813268542e-02,
    1.226825957164825e-02, 8.981720927353037e-03, 6.543687151896896e-03, 5.042565886563960e-03,
    4.529500990800284e-03
};
std::vector<double> c2_dsync2_a = {
    1.000000000000000e+00
};

Filter f2_dsync(c2_dsync2_b, c2_dsync2_a);

std::vector<double> c2_dsync4_b = {
    7.303869295435694e-03, 9.373681812569545e-03, 1.536907679358834e-02, 2.471693290630484e-02,
    3.651066972938140e-02, 4.959929457148313e-02, 6.270094081265258e-02, 7.452962766411167e-02,
    8.392262835864089e-02, 8.995575354480929e-02, 9.203504902204508e-02, 8.995575354480931e-02,
    8.392262835864092e-02, 7.452962766411168e-02, 6.270094081265259e-02, 4.959929457148314e-02,
    3.651066972938141e-02, 2.471693290630484e-02, 1.536907679358834e-02, 9.373681812569545e-03,
    7.303869295435694e-03
};
std::vector<double> c2_dsync4_a = {
    1.000000000000000e+00
};

Filter f2_dsync4(c2_dsync4_b, c2_dsync4_a);

std::vector<double> c2_dsync10_b = {
    4.557803338423001e-03, 5.069474325791004e-03, 6.573035815238145e-03, 9.014902485437780e-03,
    1.230462007835800e-02, 1.631831041800225e-02, 2.090344220074396e-02, 2.588474191483322e-02,
    3.107101346089618e-02, 3.626259841086699e-02, 4.125918284918357e-02, 4.586764245028043e-02,
    4.990961545330831e-02, 5.322850359659036e-02, 5.569562346903135e-02, 5.721526423807723e-02,
    5.772845098987632e-02, 5.721526423807723e-02, 5.569562346903136e-02, 5.322850359659036e-02,
    4.990961545330831e-02, 4.586764245028043e-02, 4.125918284918359e-02, 3.626259841086701e-02,
    3.107101346089619e-02, 2.588474191483322e-02, 2.090344220074396e-02, 1.631831041800226e-02,
    1.230462007835800e-02, 9.014902485437780e-03, 6.573035815238150e-03, 5.069474325791006e-03,
    4.557803338423001e-03
};
std::vector<double> c2_dsync10_a = {
    1.000000000000000e+00
};

Filter f2_dsync10(c2_dsync10_b, c2_dsync10_a);

std::vector<double> c2_dsync32_b = {
    4.592475846300265e-03, 5.102418069356085e-03, 6.608945026058010e-03, 9.055477267697707e-03,
    1.234905612859623e-02, 1.636390694251489e-02, 2.094627623293839e-02, 2.592035623026675e-02,
    3.109506767872609e-02, 3.627143838875037e-02, 4.125029167881321e-02, 4.583994664914770e-02,
    4.986363162023653e-02, 5.316632308088763e-02, 5.562074469999217e-02, 5.713229350131693e-02,
    5.764270191680208e-02, 5.713229350131693e-02, 5.562074469999218e-02, 5.316632308088763e-02,
    4.986363162023653e-02, 4.583994664914770e-02, 4.125029167881324e-02, 3.627143838875038e-02,
    3.109506767872610e-02, 2.592035623026675e-02, 2.094627623293839e-02, 1.636390694251490e-02,
    1.234905612859623e-02, 9.055477267697707e-03, 6.608945026058016e-03, 5.102418069356089e-03,
    4.592475846300265e-03
};
std::vector<double> c2_dsync32_a = {
    1.000000000000000e+00
};

Filter f2_dsync32(c2_dsync32_b, c2_dsync32_a);

std::vector<double> c2_sync4_b = {
    7.303869295435694e-03, 9.373681812569545e-03, 1.536907679358834e-02, 2.471693290630484e-02,
    3.651066972938140e-02, 4.959929457148313e-02, 6.270094081265258e-02, 7.452962766411167e-02,
    8.392262835864089e-02, 8.995575354480929e-02, 9.203504902204508e-02, 8.995575354480931e-02,
    8.392262835864092e-02, 7.452962766411168e-02, 6.270094081265259e-02, 4.959929457148314e-02,
    3.651066972938141e-02, 2.471693290630484e-02, 1.536907679358834e-02, 9.373681812569545e-03,
    7.303869295435694e-03
};
std::vector<double> c2_sync4_a = {
    1.000000000000000e+00
};

Filter f2_sync4(c2_sync4_b, c2_sync4_a);

std::vector<double> c2_sync10_b = {
    4.557803338423001e-03, 5.069474325791004e-03, 6.573035815238145e-03, 9.014902485437780e-03,
    1.230462007835800e-02, 1.631831041800225e-02, 2.090344220074396e-02, 2.588474191483322e-02,
    3.107101346089618e-02, 3.626259841086699e-02, 4.125918284918357e-02, 4.586764245028043e-02,
    4.990961545330831e-02, 5.322850359659036e-02, 5.569562346903135e-02, 5.721526423807723e-02,
    5.772845098987632e-02, 5.721526423807723e-02, 5.569562346903136e-02, 5.322850359659036e-02,
    4.990961545330831e-02, 4.586764245028043e-02, 4.125918284918359e-02, 3.626259841086701e-02,
    3.107101346089619e-02, 2.588474191483322e-02, 2.090344220074396e-02, 1.631831041800226e-02,
    1.230462007835800e-02, 9.014902485437780e-03, 6.573035815238150e-03, 5.069474325791006e-03,
    4.557803338423001e-03
};
std::vector<double> c2_sync10_a = {
    1.000000000000000e+00
};

Filter f2_sync10(c2_sync10_b, c2_sync10_a);

std::vector<double> c2_nr_b = {
    1.141291975113613e-04, -1.857019211291027e-03, -4.499636864042071e-03, -5.577680979937059e-03,
    -4.423694440267179e-04, 1.309163063177155e-02, 2.861211356202848e-02, 3.029931283148555e-02,
    1.098965697652802e-03, -6.398130386469832e-02, -1.492080690537196e-01, -2.223459379380252e-01,
    7.479077367478024e-01, -2.223459379380252e-01, -1.492080690537196e-01, -6.398130386469834e-02,
    1.098965697652803e-03, 3.029931283148555e-02, 2.861211356202849e-02, 1.309163063177156e-02,
    -4.423694440267179e-04, -5.577680979937061e-03, -4.499636864042074e-03, -1.857019211291029e-03,
    1.141291975113613e-04
};
std::vector<double> c2_nr_a = {
    1.000000000000000e+00
};

Filter f2_nr(c2_nr_b, c2_nr_a);

std::vector<double> c2_nr28_b = {
    -3.807292198398354e-03, 1.043166382630295e-02, 1.813581376606307e-02, -4.262438154896548e-03,
    -4.520476805475866e-02, -3.563839187235995e-02, 4.751035796026731e-02, 9.840625099698347e-02,
    1.366646446375817e-02, -1.197142079346093e-01, -1.095831494662556e-01, 5.494565292866415e-02,
    1.568954620945392e-01, 5.494565292866415e-02, -1.095831494662556e-01, -1.197142079346093e-01,
    1.366646446375817e-02, 9.840625099698347e-02, 4.751035796026733e-02, -3.563839187235997e-02,
    -4.520476805475866e-02, -4.262438154896549e-03, 1.813581376606309e-02, 1.043166382630296e-02,
    -3.807292198398354e-03
};
std::vector<double> c2_nr28_a = {
    1.000000000000000e+00
};

Filter f2_nr28(c2_nr28_b, c2_nr28_a);

std::vector<double> c2_lp18_b = {
    -1.140610593840189e-04, 1.855910522066361e-03, 4.496950462697481e-03, 5.574350957951536e-03,
    4.421053378543315e-04, -1.308381458456714e-02, -2.859503137902866e-02, -3.028122334622485e-02,
    -1.098309586278286e-03, 6.394310534647928e-02, 1.491189879190769e-01, 2.222131915754436e-01,
    2.510556756678269e-01, 2.222131915754436e-01, 1.491189879190769e-01, 6.394310534647929e-02,
    -1.098309586278287e-03, -3.028122334622485e-02, -2.859503137902866e-02, -1.308381458456715e-02,
    4.421053378543315e-04, 5.574350957951538e-03, 4.496950462697484e-03, 1.855910522066364e-03,
    -1.140610593840189e-04
};
std::vector<double> c2_lp18_a = {
    1.000000000000000e+00
};

Filter f2_lp18(c2_lp18_b, c2_lp18_a);

std::vector<double> c2_nrc2_b = {
    -3.148569668063265e-03, -4.941974513425435e-03, -9.929538598536453e-03, -1.787793973911701e-02,
    -2.783702315543740e-02, -3.829928032339736e-02, -4.750186865627083e-02, -5.380281552534787e-02,
    9.469899799540406e-01, -5.380281552534788e-02, -4.750186865627083e-02, -3.829928032339738e-02,
    -2.783702315543741e-02, -1.787793973911701e-02, -9.929538598536455e-03, -4.941974513425439e-03,
    -3.148569668063265e-03
};
std::vector<double> c2_nrc2_a = {
    1.000000000000000e+00
};

Filter f2_nrc(c2_nrc2_b, c2_nrc2_a);

std::vector<double> c2_colorlpi_b = {
    2.267438981796600e-01, 2.267438981796600e-01
};
std::vector<double> c2_colorlpi_a = {
    1.000000000000000e+00, -5.465122036406802e-01
};

Filter f2_colorlpi(c2_colorlpi_b, c2_colorlpi_a);

std::vector<double> c2_colorlpq_b = {
    1.782847982335105e-03, 5.348543947005315e-03, 5.348543947005315e-03, 1.782847982335105e-03
};
std::vector<double> c2_colorlpq_a = {
    1.000000000000000e+00, -2.474855846726988e+00, 2.078819811047953e+00, -5.897011804622845e-01
};

Filter f2_colorlpq(c2_colorlpq_b, c2_colorlpq_a);

const int f2_colorlpi_offset = 2;
const int f2_colorlpq_offset = 14;
std::vector<double> c2_colorbp4_b = {
    3.524083455247116e-02, 5.701405570187424e-07, -2.408398699505458e-01, -7.721391739544891e-07,
    4.478385909901386e-01, -7.721391739544892e-07, -2.408398699505458e-01, 5.701405570187425e-07,
    3.524083455247116e-02
};
std::vector<double> c2_colorbp4_a = {
    1.000000000000000e+00
};

Filter f2_colorbp4(c2_colorbp4_b, c2_colorbp4_a);

std::vector<double> c2_colorbp8_b = {
    1.793619856237313e-02, 1.830493260618777e-02, 2.901791166132614e-07, -5.828484174075744e-02,
    -1.225780201867041e-01, -1.151386533377725e-01, -3.929884668654461e-07, 1.554887898923649e-01,
    2.279322267448763e-01, 1.554887898923649e-01, -3.929884668654461e-07, -1.151386533377726e-01,
    -1.225780201867041e-01, -5.828484174075743e-02, 2.901791166132614e-07, 1.830493260618779e-02,
    1.793619856237313e-02
};
std::vector<double> c2_colorbp8_a = {
    1.000000000000000e+00
};

Filter f2_colorbp8(c2_colorbp8_b, c2_colorbp8_a);

std::vector<double> c2_audioin_b = {
    6.264676297181575e-05, 5.011741037745260e-04, 1.754109363210841e-03, 3.508218726421682e-03,
    4.385273408027103e-03, 3.508218726421682e-03, 1.754109363210841e-03, 5.011741037745260e-04,
    6.264676297181575e-05
};
std::vector<double> c2_audioin_a = {
    1.000000000000000e+00, -4.296261223770345e+00, 8.605029163253322e+00, -1.030114121932771e+01,
    7.988654459506183e+00, -4.084698227805914e+00, 1.338972278884777e+00, -2.564275537013397e-01,
    2.190989428180733e-02
};

Filter f2_audioin(c2_audioin_b, c2_audioin_a);

std::vector<double> c2_leftbp_b = {
    4.350594178170077e-03, 3.343353081524372e-03, -1.073777500952160e-02, 6.809375467894310e-03,
    1.325000742922696e-02, -2.883374689763496e-02, 9.021888779772603e-03, 3.827116250507442e-02,
    -5.470668753938947e-02, 5.023399166322933e-04, 7.476449641552810e-02, -7.362913874573603e-02,
    -2.256712972745371e-02, 1.081863992377557e-01, -7.307596205518978e-02, -5.219545078331883e-02,
    1.217792293188160e-01, -5.219545078331883e-02, -7.307596205518979e-02, 1.081863992377557e-01,
    -2.256712972745372e-02, -7.362913874573603e-02, 7.476449641552813e-02, 5.023399166322935e-04,
    -5.470668753938947e-02, 3.827116250507442e-02, 9.021888779772601e-03, -2.883374689763498e-02,
    1.325000742922697e-02, 6.809375467894310e-03, -1.073777500952160e-02, 3.343353081524374e-03,
    4.350594178170077e-03
};
std::vector<double> c2_leftbp_a = {
    1.000000000000000e+00
};

Filter f2_leftbp(c2_leftbp_b, c2_leftbp_a);

std::vector<double> c2_rightbp_b = {
    -3.344787284659095e-04, 5.326122609378981e-03, -1.062063536469220e-02, 1.319272573678146e-02,
    -7.526868009955770e-03, -9.515227663022494e-03, 3.297088673017456e-02, -4.895470189529475e-02,
    4.174697639258265e-02, -5.524633741995794e-03, -4.753368515984026e-02, 9.023945938850933e-02,
    -9.494734685492430e-02, 5.148656444228707e-02, 2.352675687230025e-02, -9.349039122306162e-02,
    1.218736343286969e-01, -9.349039122306162e-02, 2.352675687230025e-02, 5.148656444228707e-02,
    -9.494734685492431e-02, 9.023945938850933e-02, -4.753368515984028e-02, -5.524633741995796e-03,
    4.174697639258265e-02, -4.895470189529475e-02, 3.297088673017456e-02, -9.515227663022501e-03,
    -7.526868009955771e-03, 1.319272573678146e-02, -1.062063536469221e-02, 5.326122609378984e-03,
    -3.344787284659095e-04
};
std::vector<double> c2_rightbp_a = {
    1.000000000000000e+00
};

Filter f2_rightbp(c2_rightbp_b, c2_rightbp_a);

std::vector<double> c2_audiolp_b = {
    1.103668291221766e-11, 8.829346329774130e-11, 3.090271215420945e-10, 6.180542430841890e-10,
    7.725678038552363e-10, 6.180542430841890e-10, 3.090271215420945e-10, 8.829346329774130e-11,
    1.103668291221766e-11
};
std::vector<double> c2_audiolp_a = {
    1.000000000000000e+00, -7.550146204334163e+00, 2.495159963175967e+01, -4.714203157763692e+01,
    5.569274730841198e+01, -4.212744248979740e+01, 1.992524061941851e+01, -5.387559298917377e+00,
    6.375920139210935e-01
};

Filter f2_audiolp(c2_audiolp_b, c2_audiolp_a);

std::vector<double> c2_audiolp20_b = {
    1.468003814358643e-06, 1.174403051486914e-05, 4.110410680204199e-05, 8.220821360408399e-05,
    1.027602670051050e-04, 8.220821360408399e-05, 4.110410680204199e-05, 1.174403051486914e-05,
    1.468003814358643e-06
};
std::vector<double> c2_audiolp20_a = {
    1.000000000000000e+00, -5.842265681731639e+00, 1.515746116298822e+01, -2.276500846449473e+01,
    2.161624934688777e+01, -1.327255888620213e+01, 5.141324339405003e+00, -1.147826229544440e+00,
    1.130002216684357e-01
};

Filter f2_audiolp20(c2_audiolp20_b, c2_audiolp20_a);

std::vector<double> c2_a500_48k_b = {
    9.180235494788952e-01, -3.672094197915581e+00, 5.508141296873371e+00, -3.672094197915581e+00,
    9.180235494788952e-01
};
std::vector<double> c2_a500_48k_a = {
    1.000000000000000e+00, -3.828986095665020e+00, 5.501429593307183e+00, -3.515193865291172e+00,
    8.427672373989403e-01
};

Filter f2_a500_48k(c2_a500_48k_b, c2_a500_48k_a);

std::vector<double> c2_a500_44k_b = {
    -1.720382225986334e-03, -2.505585658275188e-03, -4.730348153602203e-03, -8.093603282119598e-03,
    -1.210053119496442e-02, -1.614086472567362e-02, -1.958776236999286e-02, -2.190064570365665e-02,
    9.789966491496416e-01, -2.190064570365666e-02, -1.958776236999286e-02, -1.614086472567363e-02,
    -1.210053119496443e-02, -8.093603282119598e-03, -4.730348153602204e-03, -2.505585658275191e-03,
    -1.720382225986334e-03
};
std::vector<double> c2_a500_44k_a = {
    1.000000000000000e+00
};

Filter f2_a500_44k(c2_a500_44k_b, c2_a500_44k_a);

std::vector<double> c2_a40h_48k_b = {
    9.931821905998739e-01, -3.972728762399496e+00, 5.959093143599244e+00, -3.972728762399496e+00,
    9.931821905998739e-01
};
std::vector<double> c2_a40h_48k_a = {
    1.000000000000000e+00, -3.986317712211590e+00, 5.959046661447476e+00, -3.959139812214155e+00,
    9.864108637247646e-01
};

Filter f2_a40h_48k(c2_a40h_48k_b, c2_a40h_48k_a);

std::vector<double> c2_hilbertr_b = {
    -1.851851851851851e-02, -1.851851851851858e-02, -1.851851851851844e-02, -1.851851851851872e-02,
    -1.851851851851852e-02, -1.851851851851849e-02, -1.851851851851853e-02, -1.851851851851849e-02,
    -1.851851851851843e-02, -1.851851851851853e-02, -1.851851851851844e-02, -1.851851851851846e-02,
    -1.851851851851794e-02, 4.814814814814815e-01, -1.851851851851872e-02, -1.851851851851850e-02,
    -1.851851851851857e-02, -1.851851851851854e-02, -1.851851851851870e-02, -1.851851851851853e-02,
    -1.851851851851857e-02, -1.851851851851865e-02, -1.851851851851852e-02, -1.851851851851849e-02,
    -1.851851851851857e-02, -1.851851851851850e-02, -1.851851851851852e-02
};
std::vector<double> c2_hilbertr_a = {
    1.000000000000000e+00
};

Filter f2_hilbertr(c2_hilbertr_b, c2_hilbertr_a);

std::vector<double> c2_hilberti_b = {
    -1.962848289588819e-02, 1.553888205883857e-02, -2.487468723645290e-02, 1.217982123436036e-02,
    -3.207501495497922e-02, 9.300349555976783e-03, -4.293075142238721e-02, 6.740189523448193e-03,
    -6.185615955811687e-02, 4.388969514287131e-03, -1.050237374003280e-01, 2.164504384410190e-03,
    -3.179506838793675e-01, 0.000000000000000e+00, 3.179506838793673e-01, -2.164504384410416e-03,
    1.050237374003280e-01, -4.388969514286931e-03, 6.185615955811677e-02, -6.740189523448237e-03,
    4.293075142238729e-02, -9.300349555976842e-03, 3.207501495497922e-02, -1.217982123436019e-02,
    2.487468723645292e-02, -1.553888205883851e-02, 1.962848289588827e-02
};
std::vector<double> c2_hilberti_a = {
    1.000000000000000e+00
};

Filter f2_hilberti(c2_hilberti_b, c2_hilberti_a);

std::vector<double> c2_pilot_b = {
    1.817901843543287e-02, 6.802041749100404e-17, -4.879895376103990e-02, 4.305004418889394e-16,
    1.227256007473350e-01, 1.693858423095544e-15, -1.966562944890921e-01, -4.565555273818813e-15,
    2.272802651342001e-01, -4.565555273818813e-15, -1.966562944890921e-01, 1.693858423095545e-15,
    1.227256007473350e-01, 4.305004418889393e-16, -4.879895376103992e-02, 6.802041749100411e-17,
    1.817901843543287e-02
};
std::vector<double> c2_pilot_a = {
    1.000000000000000e+00
};

Filter f2_pilot(c2_pilot_b, c2_pilot_a);

std::vector<double> c2_fmdeemp_b = {
    4.795408695004655e-05, 7.097070232758687e-05, 1.162983920200813e-04, 2.377538100602406e-04,
    4.036970192157422e-04, 7.910534993095139e-04, 1.308086972794372e-03, 2.329168150063381e-03,
    3.867050819971179e-03, 6.323290047668001e-03, 1.047733988469226e-02, 1.648258718313019e-02,
    2.679862393683207e-02, 4.292534896850254e-02, 6.966728643948848e-02, 1.324241229048471e-01,
    2.874090238495433e-01, 1.324241229048468e-01, 6.966728643948851e-02, 4.292534896850245e-02,
    2.679862393683206e-02, 1.648258718313015e-02, 1.047733988469226e-02, 6.323290047667920e-03,
    3.867050819971209e-03, 2.329168150063344e-03, 1.308086972794367e-03, 7.910534993095119e-04,
    4.036970192157484e-04, 2.377538100602358e-04, 1.162983920200774e-04, 7.097070232758928e-05,
    4.795408695005012e-05
};
std::vector<double> c2_fmdeemp_a = {
    1.000000000000000e+00
};

Filter f2_fmdeemp(c2_fmdeemp_b, c2_fmdeemp_a);

std::vector<double> c2_efm8_b = {
    -9.111535510294280e-04, -1.251936114258104e-03, -1.732032006080744e-03, -2.385611144070485e-03,
    -3.204766378338999e-03, -4.127815310301764e-03, -5.033928296543916e-03, -5.745491521878144e-03,
    -6.038724679420682e-03, -5.662068221287680e-03, -4.360853906609532e-03, -1.905896801007799e-03,
    1.876991760128046e-03, 7.079886061156261e-03, 1.369128019537608e-02, 2.158309253498079e-02,
    3.050817543700797e-02, 4.010931759902071e-02, 4.993952650499034e-02, 5.949215855288342e-02,
    6.823836254804105e-02, 7.566844529674076e-02, 8.133325725830043e-02, 8.488159489598222e-02,
    8.608994206986433e-02, 8.488159489598222e-02, 8.133325725830043e-02, 7.566844529674077e-02,
    6.823836254804107e-02, 5.949215855288342e-02, 4.993952650499035e-02, 4.010931759902073e-02,
    3.050817543700798e-02, 2.158309253498079e-02, 1.369128019537608e-02, 7.079886061156261e-03,
    1.876991760128046e-03, -1.905896801007799e-03, -4.360853906609534e-03, -5.662068221287684e-03,
    -6.038724679420682e-03, -5.745491521878151e-03, -5.033928296543917e-03, -4.127815310301767e-03,
    -3.204766378339002e-03, -2.385611144070484e-03, -1.732032006080746e-03, -1.251936114258104e-03,
    -9.111535510294280e-04
};
std::vector<double> c2_efm8_a = {
    1.000000000000000e+00
};

Filter f2_efm8(c2_efm8_b, c2_efm8_a);

std::vector<double> c2_syncid8_b = {
    3.081237304443338e-08, 9.243711913330015e-08, 9.243711913330015e-08, 3.081237304443338e-08
};
std::vector<double> c2_syncid8_a = {
    1.000000000000000e+00, -2.987433650055722e+00, 2.974946132665443e+00, -9.875122361107359e-01
};

Filter f2_syncid8(c2_syncid8_b, c2_syncid8_a);

std::vector<double> c2_syncid4_b = {
    2.449622763746039e-07, 7.348868291238116e-07, 7.348868291238116e-07, 2.449622763746039e-07
};
std::vector<double> c2_syncid4_a = {
    1.000000000000000e+00, -2.974867424113648e+00, 2.950049679327468e+00, -9.751802955156089e-01
};

Filter f2_syncid4(c2_syncid4_b, c2_syncid4_a);

std::vector<double> c2_syncid32_b = {
    2.247629572785221e-08, 6.742888718355662e-08, 6.742888718355662e-08, 2.247629572785221e-08
};
std::vector<double> c2_syncid32_a = {
    1.000000000000000e+00, -2.988690281515673e+00, 2.977444427485293e+00, -9.887539661592547e-01
};

Filter f2_syncid32(c2_syncid32_b, c2_syncid32_a);

std::vector<double> c2_syncid10_b = {
    1.579571604101607e-08, 4.738714812304821e-08, 4.738714812304821e-08, 1.579571604101607e-08
};
std::vector<double> c2_syncid10_a = {
    1.000000000000000e+00, -2.989946914091736e+00, 2.979944296951953e+00, -9.899972564944884e-01
};

Filter f2_syncid10(c2_syncid10_b, c2_syncid10_a);

const int syncid4_offset = 165;
const int syncid8_offset = 320;
const int syncid32_offset = 360;
const int syncid10_offset = 400;
std::vector<double> c2_linelen_b = {
    2.539993835013455e-03, 5.744201059608381e-03, 1.470833651484297e-02, 3.145606087175080e-02,
    5.548225080960399e-02, 8.344191096544862e-02, 1.098889114382941e-01, 1.288595816690949e-01,
    1.357575056726855e-01, 1.288595816690949e-01, 1.098889114382941e-01, 8.344191096544866e-02,
    5.548225080960400e-02, 3.145606087175079e-02, 1.470833651484297e-02, 5.744201059608387e-03,
    2.539993835013455e-03
};
std::vector<double> c2_linelen_a = {
    1.000000000000000e+00
};

Filter f2_linelen(c2_linelen_b, c2_linelen_a);

#endif // DEEMP2_H
