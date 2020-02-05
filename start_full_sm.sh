#!/bin/bash
./../build_lcde_r/lcde -p 55000 &
pid_lcde=$!
./../build_keyboarde_r/keyboarde -p 56000 & 
pid_keyboarde=$!
./../build_aye_r/aye -p 52000 &
pid_aye1=$!
./../build_aye_r/aye -p 52001 &
pid_aye2=$!

trap 'kill $pid_lcde && kill $pid_keyboarde && kill $pid_aye1 && kill $pid_aye2' SIGINT

sudo mount -t auto resurse/microsd.img resurse/microsd
sleep 2
sudo cp -r lua_scripts resurse/microsd
sudo umount resurse/microsd

sleep 1
./../build_aym_soft_r/aym