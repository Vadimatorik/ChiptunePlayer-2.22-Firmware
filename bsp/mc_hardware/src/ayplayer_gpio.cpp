#include "ayplayer_mc_hardware.h"

extern const mc::PinCfg		buttonIncCfg;
extern const mc::PinCfg		buttonDecCfg;
mc::PinMultifuncIt			buttonInc				( &buttonIncCfg, 1, GPIO_PIN_7 );
mc::PinMultifuncIt			buttonDec				( &buttonDecCfg, 1, GPIO_PIN_8 );

extern const mc::PinCfg		lcdResCfg;
extern const mc::PinCfg		lcdDcCfg;
extern const mc::PinCfg		lcdCsCfg;
mc::Pin 					lcdRes					( &lcdResCfg );
mc::Pin						lcdDc					( &lcdDcCfg );						/// Данные/команда.
mc::Pin						lcdCs					( &lcdCsCfg );

extern const mc::PinCfg		sdSetCfg;
extern const mc::PinCfg		sdReadCfg;
mc::Pin						sdSet					( &sdSetCfg );
mc::PinMultifuncIt			sdRead					( &sdReadCfg, 1, GPIO_PIN_11 );

extern const mc::PinCfg		boardGpio2;
extern const mc::PinCfg		boardGpio3;
extern const mc::PinCfg		boardGpio4;
mc::Pin						shStrob					( &boardGpio2 );
mc::Pin						ad5204Cs				( &boardGpio3 );
mc::Pin						ltcCs					( &boardGpio4 );

extern const mc::PinCfg		pwr5vOnCfg;
extern const mc::PinCfg		pwrOnCfg;
mc::Pin						pwr5vOn					( &pwr5vOnCfg );
mc::Pin						pwrOn					( &pwrOnCfg );

extern const mc::PinCfg		tpStCfg;
extern const mc::PinCfg		tpChCfg;
mc::Pin						tpSt					( &tpStCfg );
mc::Pin						tpCh					( &tpChCfg );

extern const mc::PinCfg		br0Cfg;
extern const mc::PinCfg		br1Cfg;
extern const mc::PinCfg		br2Cfg;
extern const mc::PinCfg		bc0Cfg;
extern const mc::PinCfg		bc1Cfg;
extern const mc::PinCfg		bc2Cfg;

mc::Pin						keyboardStrings[ 3 ] = {
	mc::Pin( &br0Cfg ),
	mc::Pin( &br1Cfg ),
	mc::Pin( &br2Cfg )
};

mc::Pin						keyboardColumns[ 3 ] = {
	mc::Pin( &bc0Cfg ),
	mc::Pin( &bc1Cfg ),
	mc::Pin( &bc2Cfg )
};


