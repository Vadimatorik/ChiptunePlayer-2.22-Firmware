#!/bin/bash
./../lcde/cmake-build-debug/lcde &
./../build_keyboarde/keyboarde & 

sudo mount -t auto resurse/microsd.img resurse/microsd
sleep 3
sudo cp -r lua_scripts resurse/microsd
sudo umount resurse/microsd

sleep 1
./../build_aym_soft_d/aym