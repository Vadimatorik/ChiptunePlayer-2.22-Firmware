rm -R ../build_aym_soft_r/*
rm -R ../build_aye_r/*
rm -R ../build_lcde_r/*
rm -R ../build_keyboarde_r/*

cd ../build_aym_soft_r && cmake ../ChiptunePlayer-2.22-Firmware/c_code -DCMAKE_BUILD_TYPE="SOFT" && make -j
cd ../build_aye_r && cmake ../aye -DCMAKE_BUILD_TYPE=Release && make -j
cd ../build_lcde_r && cmake ../lcde -DCMAKE_BUILD_TYPE=Release && make -j
cd ../build_keyboarde_r && cmake ../keyboarde -DCMAKE_BUILD_TYPE=Release && make -j