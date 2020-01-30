#!/bin/bash
./../build_lcde_r/lcde &
./../build_keyboarde_r/keyboarde &
./../build_aye_r/aye -p 52000 &
./../build_aye_r/aye -p 52001 &

sudo mount -t auto resurse/microsd.img resurse/microsd
sleep 2
sudo cp -r lua_scripts resurse/microsd
sudo umount resurse/microsd