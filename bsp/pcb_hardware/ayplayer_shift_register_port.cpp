#include "shift_register_8bit_port.h"
#include "ayplayer_pcb_hardware.h"

const ShiftRegisterPort8bitCfg chip1Port = {
	.byte	=	0,
	.sr		=	&boardShiftRegisters
};

const ShiftRegisterPort8bitCfg chip2Port = {
	.byte	=	1,
	.sr		=	&boardShiftRegisters
};

ShiftRegisterPort8bit ayPorts[ 2 ] = {
	ShiftRegisterPort8bit( &chip1Port ),
	ShiftRegisterPort8bit( &chip2Port )
};
