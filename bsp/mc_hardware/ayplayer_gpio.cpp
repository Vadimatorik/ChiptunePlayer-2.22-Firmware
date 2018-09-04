#include "ayplayer_mc_hardware.h"

extern const McHardwareInterfacesImplementation::PinCfg		buttonIncCfg;
extern const McHardwareInterfacesImplementation::PinCfg		buttonDecCfg;
McHardwareInterfacesImplementation::PinMultifuncIt			buttonInc				( &buttonIncCfg, 1, GPIO_PIN_7 );
McHardwareInterfacesImplementation::PinMultifuncIt			buttonDec				( &buttonDecCfg, 1, GPIO_PIN_8 );

extern const McHardwareInterfacesImplementation::PinCfg		lcdResCfg;
extern const McHardwareInterfacesImplementation::PinCfg		lcdDcCfg;
extern const McHardwareInterfacesImplementation::PinCfg		lcdCsCfg;
McHardwareInterfacesImplementation::Pin 					lcdRes					( &lcdResCfg );
McHardwareInterfacesImplementation::Pin						lcdDc					( &lcdDcCfg );						/// Данные/команда.
McHardwareInterfacesImplementation::Pin						lcdCs					( &lcdCsCfg );

extern const McHardwareInterfacesImplementation::PinCfg		sdPushCfg;
extern const McHardwareInterfacesImplementation::PinCfg		sdCheckerValueSetter;
McHardwareInterfacesImplementation::Pin						sdSignal				( &sdCheckerValueSetter );
McHardwareInterfacesImplementation::PinMultifuncIt			sdIn					( &sdPushCfg, 1, GPIO_PIN_11 );

extern const McHardwareInterfacesImplementation::PinCfg		boardGpio2;
extern const McHardwareInterfacesImplementation::PinCfg		boardGpio3;
extern const McHardwareInterfacesImplementation::PinCfg		boardGpio4;
McHardwareInterfacesImplementation::Pin						shStrob					( &boardGpio2 );
McHardwareInterfacesImplementation::Pin						ad5204Cs				( &boardGpio3 );
McHardwareInterfacesImplementation::Pin						ltcCs					( &boardGpio4 );

extern const McHardwareInterfacesImplementation::PinCfg		pwr5vOnCfg;
extern const McHardwareInterfacesImplementation::PinCfg		pwrOnCfg;
McHardwareInterfacesImplementation::Pin						pwr5vOn					( &pwr5vOnCfg );
McHardwareInterfacesImplementation::Pin						pwrOn					( &pwrOnCfg );

extern const McHardwareInterfacesImplementation::PinCfg		tpStCfg;
extern const McHardwareInterfacesImplementation::PinCfg		tpChCfg;
McHardwareInterfacesImplementation::Pin						tpSt					( &tpStCfg );
McHardwareInterfacesImplementation::Pin						tpCh					( &tpChCfg );

extern const McHardwareInterfacesImplementation::PinCfg		br0Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		br1Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		br2Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		bc0Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		bc1Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		bc2Cfg;

McHardwareInterfacesImplementation::Pin						keyboardStrings[ 3 ] = {
	McHardwareInterfacesImplementation::Pin( &br0Cfg ),
	McHardwareInterfacesImplementation::Pin( &br1Cfg ),
	McHardwareInterfacesImplementation::Pin( &br2Cfg )
};
McHardwareInterfacesImplementation::Pin						keyboardColumns[ 3 ] = {
	McHardwareInterfacesImplementation::Pin( &bc0Cfg ),
	McHardwareInterfacesImplementation::Pin( &bc1Cfg ),
	McHardwareInterfacesImplementation::Pin( &bc2Cfg )
};
