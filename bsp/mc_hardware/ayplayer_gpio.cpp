#include "ayplayer_mc_hardware.h"

extern const pinCfg		buttonIncCfg;
extern const pinCfg		buttonDecCfg;
PinMultifuncIt			buttonInc				( &buttonIncCfg, 1, GPIO_PIN_7 );
PinMultifuncIt			buttonDec				( &buttonDecCfg, 1, GPIO_PIN_8 );

extern const pinCfg		lcdResCfg;
extern const pinCfg		lcdDcCfg;
extern const pinCfg		lcdCsCfg;
Pin 					lcdRes					( &lcdResCfg );
Pin						lcdDc					( &lcdDcCfg );						/// Данные/команда.
Pin						lcdCs					( &lcdCsCfg );

extern const pinCfg		sdPushCfg;
extern const pinCfg		sdCheckerValueSetter;
Pin						sdSignal				( &sdCheckerValueSetter );
PinMultifuncIt			sdIn					( &sdPushCfg, 1, GPIO_PIN_11 );

extern const pinCfg		boardGpio2;
extern const pinCfg		boardGpio3;
extern const pinCfg		boardGpio4;
Pin						shStrob					( &boardGpio2 );
Pin						ad5204Cs				( &boardGpio3 );
Pin						ltcCs					( &boardGpio4 );

extern const pinCfg		pwr5vOnCfg;
extern const pinCfg		pwrOnCfg;
Pin						pwr5vOn					( &pwr5vOnCfg );
Pin						pwrOn					( &pwrOnCfg );

extern const pinCfg		tpStCfg;
extern const pinCfg		tpChCfg;
Pin						tpSt					( &tpStCfg );
Pin						tpCh					( &tpChCfg );

extern const pinCfg		br0Cfg;
extern const pinCfg		br1Cfg;
extern const pinCfg		br2Cfg;
extern const pinCfg		bc0Cfg;
extern const pinCfg		bc1Cfg;
extern const pinCfg		bc2Cfg;
Pin						keyboardStrings[ 3 ] = {
	Pin( &br0Cfg ),		Pin( &br1Cfg ),		Pin( &br2Cfg )
};
Pin						keyboardColumns[ 3 ] = {
	Pin( &bc0Cfg ),		Pin( &bc1Cfg ),		Pin( &bc2Cfg )
};
