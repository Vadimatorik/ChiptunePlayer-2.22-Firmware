#include "ayplayer_class/structs/ayplayer_struct_gpio_cfg.h"
#include "ayplayer_mc_hardware.h"
#include "ayplayer_pcb_hardware.h"

extern const AyplayerGpio gpio = {
	.pwr = {
		.pwr5v				=	&pwr5vOn,
		.allPwr				=	&pwrOn,
	},
	.vr = {
		.inc				=	&buttonInc,
		.dec				=	&buttonDec
	},
	.sd = {
		.read				=	&sdRead,
		.it					=	&sdRead,
		.set				=	&sdSet
	}
};
