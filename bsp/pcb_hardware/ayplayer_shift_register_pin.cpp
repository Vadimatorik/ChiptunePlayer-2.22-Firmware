#include "shift_register_pin.h"
#include "ayplayer_pcb_hardware.h"

const ShiftRegisterPinCfg pwrAy1Cfg = {
	.byte		=	2,
	.bit		=	0,
	.sr			=	&boardShiftRegisters
};

const ShiftRegisterPinCfg pwrAy2Cfg = {
	.byte		=	2,
	.bit		=	1,
	.sr			=	&boardShiftRegisters
};

const ShiftRegisterPinCfg ay1ResCfg = {
	.byte		=	2,
	.bit		=	2,
	.sr			=	&boardShiftRegisters
};

const ShiftRegisterPinCfg ay2ResCfg = {
	.byte		=	2,
	.bit		=	3,
	.sr			=	&boardShiftRegisters
};

const ShiftRegisterPinCfg bc1Cfg = {
	.byte		=	2,
	.bit		=	4,
	.sr			=	&boardShiftRegisters
};

const ShiftRegisterPinCfg bdirCfg = {
	.byte		=	2,
	.bit		=	5,
	.sr			=	&boardShiftRegisters
};

ShiftRegisterPin pwrAy1		( &pwrAy1Cfg );
ShiftRegisterPin pwrAy2		( &pwrAy2Cfg );
ShiftRegisterPin ay1Res		( &ay1ResCfg );
ShiftRegisterPin ay2Res		( &ay2ResCfg );
ShiftRegisterPin bc1		( &bc1Cfg );
ShiftRegisterPin bdir		( &bdirCfg );
