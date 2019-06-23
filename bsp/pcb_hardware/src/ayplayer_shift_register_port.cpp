#include "ayplayer_pcb_hardware.h"
#include "shift_register_8bit_port.h"

const ShiftRegister::Port8bitCfg chip1Port = {
    .byte    =    2,
    .sr        =    &boardShiftRegisters
};

ShiftRegister::Port8bit chip1(&chip1Port);

const ShiftRegister::Port8bitCfg chip2Port = {
    .byte    =    1,
    .sr        =    &boardShiftRegisters
};

ShiftRegister::Port8bit chip2(&chip2Port);

McHardwareInterfaces::Port8bit *ayPorts[2] = {
    &chip1,
    &chip2
};
