#include "shift_register_pin.h"
#include "ayplayer_pcb_hardware.h"

const ShiftRegister::PinCfg pwrAy1Cfg = {
    .byte        =    0,
    .bit        =    0,
    .sr            =    &boardShiftRegisters
};

const ShiftRegister::PinCfg pwrAy2Cfg = {
    .byte        =    0,
    .bit        =    1,
    .sr            =    &boardShiftRegisters
};

const ShiftRegister::PinCfg ay1ResCfg = {
    .byte        =    0,
    .bit        =    2,
    .sr            =    &boardShiftRegisters
};

const ShiftRegister::PinCfg ay2ResCfg = {
    .byte        =    0,
    .bit        =    3,
    .sr            =    &boardShiftRegisters
};

const ShiftRegister::PinCfg bc1Cfg = {
    .byte        =    0,
    .bit        =    4,
    .sr            =    &boardShiftRegisters
};

const ShiftRegister::PinCfg bdirCfg = {
    .byte        =    0,
    .bit        =    5,
    .sr            =    &boardShiftRegisters
};

const ShiftRegister::PinCfg shdnCfg = {
    .byte        =    0,
    .bit        =    6,
    .sr            =    &boardShiftRegisters
};

ShiftRegister::Pin pwrAy[2] = {
    ShiftRegister::Pin(&pwrAy1Cfg),
    ShiftRegister::Pin(&pwrAy2Cfg),
};

ShiftRegister::Pin ay1Res(&ay1ResCfg);
ShiftRegister::Pin ay2Res(&ay2ResCfg);
ShiftRegister::Pin bc1(&bc1Cfg);
ShiftRegister::Pin bdir(&bdirCfg);
ShiftRegister::Pin shdn(&shdnCfg);

