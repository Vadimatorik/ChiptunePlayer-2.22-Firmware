#!/bin/bash
./../build_lcde_r/lcde &
./../build_keyboarde_r/keyboarde & 

sudo mount -t auto resurse/microsd.img resurse/microsd
sleep 2
sudo cp -r lua_scripts resurse/microsd
sudo umount resurse/microsd

sleep 3
./../build_aym_soft_r/aym