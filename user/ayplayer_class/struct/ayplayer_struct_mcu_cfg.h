#pragma once

#include "ayplayer_mc_hardware.h"
#include "ayplayer_struct_gpio_cfg.h"

struct AyPlayerMcuCfg {
	WdtBase*									const	wdt;
	AyplayerGpio*								const	gpio;
	GlobalPortBase*								const	gp;
	UartBase*									const	debugUart;
	RccBase*									const	rcc;
	SpiMaster8BitBase*							const	spiLcd;
	SpiMaster8BitBase*							const	spiBoard;
	AdcOneChannelBase*							const	adcBat;
	TimPwmOneChannelBase*						const	lcdPwmTim;
	TimInterruptBase*							const	interruptAyTim;
	Pwr*										const	pwr;

#ifdef configGENERATE_RUN_TIME_STATS
	TimCounter*									const	timRunTimeStatus;
#endif
};
