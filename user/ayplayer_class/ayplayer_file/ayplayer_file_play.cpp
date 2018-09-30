#include "ayplayer_ay_file_class.h"
#include "ayplayer_pcb_hardware.h"
#include "ayplayer_mc_hardware.h"

void	setPwr	(	uint32_t			chipNumber,
					bool				state	) {
	pwr5vOn.set( state );
	ltc.init();
	ltc.setFrequency( 0, WaveGenerators::LTC6903OutputMode::CLK_ON_INV_OFF );
	shdn.set();
	pwrAy[ chipNumber ].set( state );
}

AyYmFilePlayCfg ayYmFilePlayCfg = {
	.ayLow			=	&ayLow,
	.setPwr			=	&setPwr
};

AyYmFilePlay		ayYmFilePlay( &ayYmFilePlayCfg );
