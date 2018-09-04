#pragma once

#include "ayplayer_mc_hardware.h"
#include "ayplayer_struct_gpio_cfg.h"

struct AyPlayerMcuCfg {
	McHardwareInterfaces::Wdt*						const	wdt;
	const AyplayerGpio*								const	gpio;
	McHardwareInterfaces::GlobalPort*				const	gp;
	McHardwareInterfaces::Uart*						const	debugUart;
	McHardwareInterfaces::Rcc*						const	rcc;
	McHardwareInterfaces::SpiMaster8Bit*			const	spiLcd;
	McHardwareInterfaces::SpiMaster8Bit*			const	spiBoard;
	McHardwareInterfaces::AdcOneChannel*			const	adcBat;
	McHardwareInterfaces::TimPwmOneChannel*			const	lcdPwmTim;
	McHardwareInterfaces::TimInterrupt*				const	interruptAyTim;
	McHardwareInterfacesImplementation::Pwr*		const	pwr;

#ifdef configGENERATE_RUN_TIME_STATS
	McHardwareInterfacesImplementation::TimCounter*	const	timRunTimeStatus;
#endif
};
