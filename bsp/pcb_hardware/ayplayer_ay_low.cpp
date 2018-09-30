#include "ayplayer_os_object.h"
#include "ayplayer_pcb_hardware.h"
#include "ay_ym_low_lavel.h"
#include "ayplayer_mc_hardware.h"

ayYmConnectionChipCfg ay1ConnectCfg = { 0, 1, 2, 3, 4, 5, 6, 7 };
ayYmConnectionChipCfg ay2ConnectCfg = { 0, 1, 2, 3, 4, 5, 6, 7 };

ayYmConnectionChipCfg arrayConnectCfg[] = {
	ay1ConnectCfg,
	ay2ConnectCfg
};

void ayplayerFuncFrequencyAyControl ( bool state ) {
	if ( state ) {
		ltc.setFrequency( 1.75e6, WaveGenerators::LTC6903OutputMode::CLK_ON_INV_OFF );
	} else {
		ltc.setFrequency( 1.75e6, WaveGenerators::LTC6903OutputMode::CLK_OFF_INV_OFF );
	}
}

void ayplayerFuncTimInterruptTask ( bool state ) {
	interruptAy.setState( state );
}

const ayYmLowLavelCfg ayLowCfg = {
	.mutex						=	nullptr,
	.ports						=	ayPorts,
	.semaphoreSecOut			=	&osData.sPlayTic,
	.bdir						=	&bdir,
	.bc1						=	&bc1,
	.queueArray					=	osData.qAyLow,
	.ayNumber					=	2,
	.connectCfg					=	arrayConnectCfg,
	.taskPrio					=	4,
	.funcFrequencyAyControl		=	ayplayerFuncFrequencyAyControl,
	.funcTimInterruptTask		=	ayplayerFuncTimInterruptTask
};

AyYmLowLavel ayLow( &ayLowCfg );
