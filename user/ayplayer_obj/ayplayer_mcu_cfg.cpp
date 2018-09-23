#include "../ayplayer_class/structs/ayplayer_struct_mcu_cfg.h"
#include "ayplayer_mc_hardware.h"

extern const AyplayerGpio		gpio;

extern const AyPlayerMcuCfg ayMcuCfg = {
	.wdt								=	&wdt,
	.gpio								=	&gpio,
	.gp									=	&gp,
	.debugUart							=	&usart,
	.rcc								=	&rcc,
	.spiLcd								=	&lcdSpi,
	.spiBoard							=	&boardSpi,
	.adcBat								=	&adcBat,
	.lcdPwmTim							=	&lcdPwm,
	.interruptAyTim						=	&interruptAy,
	.pwr								=	&pwr,

#ifdef configGENERATE_RUN_TIME_STATS
	.timRunTimeStatus					=	&timRunTimeStats
#endif
};
