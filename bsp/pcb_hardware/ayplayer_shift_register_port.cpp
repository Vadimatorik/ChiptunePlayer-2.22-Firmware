#include "ayplayer_pcb_hardware.h"
#include "shift_register_8bit_port.h"

const ShiftRegister::Port8bitCfg chip1Port = {
	.byte	=	0,
	.sr		=	&boardShiftRegisters
};

const ShiftRegister::Port8bitCfg chip2Port = {
	.byte	=	1,
	.sr		=	&boardShiftRegisters
};

ShiftRegister::Port8bit ayPorts[ 2 ] = {
	ShiftRegister::Port8bit( &chip1Port ),
	ShiftRegister::Port8bit( &chip2Port )
};
