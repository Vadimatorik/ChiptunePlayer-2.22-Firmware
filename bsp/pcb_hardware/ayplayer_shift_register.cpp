#include "shift_register.h"

#include "ayplayer_os_object.h"
#include "ayplayer_mc_hardware.h"

uint8_t boardShiftRegistersData[ 3 ];

const ShiftRegisterStaticCfg boardShiftRegistersCfg = {
	.st				=	&shStrob,
	.dataArray		=	boardShiftRegistersData,
	.arraySize		=	sizeof( boardShiftRegistersData ),
	.strobActive	=	true,
	.spiObj			=	&boardSpi,
	.mutex			=	&osData.mBoardSpi
};

ShiftRegister boardShiftRegisters( &boardShiftRegistersCfg );
