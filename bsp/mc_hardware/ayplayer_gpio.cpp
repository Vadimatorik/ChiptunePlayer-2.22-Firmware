#include "ayplayer_mc_hardware.h"

extern const PinCfg		buttonIncCfg;
extern const PinCfg		buttonDecCfg;
PinMultifuncIt			buttonInc				( &buttonIncCfg, 1, GPIO_PIN_7 );
PinMultifuncIt			buttonDec				( &buttonDecCfg, 1, GPIO_PIN_8 );

extern const PinCfg		lcdResCfg;
extern const PinCfg		lcdDcCfg;
extern const PinCfg		lcdCsCfg;
Pin 					lcdRes					( &lcdResCfg );
Pin						lcdDc					( &lcdDcCfg );						/// Данные/команда.
Pin						lcdCs					( &lcdCsCfg );

extern const PinCfg		sdPushCfg;
extern const PinCfg		sdCheckerValueSetter;
Pin						sdSignal				( &sdCheckerValueSetter );
PinMultifuncIt			sdIn					( &sdPushCfg, 1, GPIO_PIN_11 );

extern const PinCfg		boardGpio2;
extern const PinCfg		boardGpio3;
extern const PinCfg		boardGpio4;
Pin						shStrob					( &boardGpio2 );
Pin						ad5204Cs				( &boardGpio3 );
Pin						ltcCs					( &boardGpio4 );

extern const PinCfg		pwr5vOnCfg;
extern const PinCfg		pwrOnCfg;
Pin						pwr5vOn					( &pwr5vOnCfg );
Pin						pwrOn					( &pwrOnCfg );

extern const PinCfg		tpStCfg;
extern const PinCfg		tpChCfg;
Pin						tpSt					( &tpStCfg );
Pin						tpCh					( &tpChCfg );

extern const PinCfg		br0Cfg;
extern const PinCfg		br1Cfg;
extern const PinCfg		br2Cfg;
extern const PinCfg		bc0Cfg;
extern const PinCfg		bc1Cfg;
extern const PinCfg		bc2Cfg;
Pin						keyboardStrings[ 3 ] = {
	Pin( &br0Cfg ),		Pin( &br1Cfg ),		Pin( &br2Cfg )
};
Pin						keyboardColumns[ 3 ] = {
	Pin( &bc0Cfg ),		Pin( &bc1Cfg ),		Pin( &bc2Cfg )
};
