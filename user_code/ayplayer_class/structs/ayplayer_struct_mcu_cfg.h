#pragma once

#include "ayplayer_struct_gpio_cfg.h"
#include "ayplayer_mc_hardware.h"

struct AyPlayerMcuCfg {
	mc_interfaces::Wdt*						const	wdt;
	const AyplayerGpio*								const	gpio;
	mc_interfaces::PortGlobal*				const	gp;
	mc_interfaces::uart*						const	debugUart;
	mc_interfaces::clk*						const	rcc;
	mc_interfaces::SpiMaster8Bit*			const	spiLcd;
	mc_interfaces::SpiMaster8Bit*			const	spiBoard;
	mc_interfaces::AdcOneChannel*			const	adcBat;
	mc_interfaces::TimPwmOneChannel*			const	lcdPwmTim;
	mc_interfaces::TimInterrupt*				const	interruptAyTim;
	mc::Pwr*		const	pwr;

#ifdef configGENERATE_RUN_TIME_STATS
	mc::TimCounter*	const	timRunTimeStatus;
#endif
};
