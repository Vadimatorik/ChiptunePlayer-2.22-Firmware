#include "ltc6903.h"

#include "ayplayer_mc_hardware.h"

const WaveGenerators::LTC6903Cfg ltcCfg = {
	.cs		=	&ltcCs,
	.s		=	&boardSpi,
};

WaveGenerators::LTC6903	ltc( &ltcCfg );
