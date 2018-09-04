#include "port_global.h"
#include "pin.h"

extern const McHardwareInterfacesImplementation::PinCfg		adcCfg;
extern const McHardwareInterfacesImplementation::PinCfg		buttonIncCfg;
extern const McHardwareInterfacesImplementation::PinCfg		buttonDecCfg;
extern const McHardwareInterfacesImplementation::PinCfg		lcdClkCfg;
extern const McHardwareInterfacesImplementation::PinCfg		lcdPwmCfg;
extern const McHardwareInterfacesImplementation::PinCfg		lcdResCfg;
extern const McHardwareInterfacesImplementation::PinCfg		lcdDcCfg;
extern const McHardwareInterfacesImplementation::PinCfg		lcdCsCfg;
extern const McHardwareInterfacesImplementation::PinCfg		sd1SmdCfg;
extern const McHardwareInterfacesImplementation::PinCfg		sdIoAndClkCfg;
extern const McHardwareInterfacesImplementation::PinCfg		sdPushCfg;
extern const McHardwareInterfacesImplementation::PinCfg		sdCheckerValueSetter;
extern const McHardwareInterfacesImplementation::PinCfg		usbCfg;
extern const McHardwareInterfacesImplementation::PinCfg		spiBoardTxCfg;
extern const McHardwareInterfacesImplementation::PinCfg		spiBoardClkCfg;
extern const McHardwareInterfacesImplementation::PinCfg		boardGpio1;
extern const McHardwareInterfacesImplementation::PinCfg		boardGpio2;
extern const McHardwareInterfacesImplementation::PinCfg		boardGpio3;
extern const McHardwareInterfacesImplementation::PinCfg		boardGpio4;
extern const McHardwareInterfacesImplementation::PinCfg		jtagPart1Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		jtagPart1Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		pwr5vOnCfg;
extern const McHardwareInterfacesImplementation::PinCfg		pwrOnCfg;
extern const McHardwareInterfacesImplementation::PinCfg		tpStCfg;
extern const McHardwareInterfacesImplementation::PinCfg		tpChCfg;
extern const McHardwareInterfacesImplementation::PinCfg		uartDebugCfg;
extern const McHardwareInterfacesImplementation::PinCfg		br0Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		br1Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		br2Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		bc0Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		bc1Cfg;
extern const McHardwareInterfacesImplementation::PinCfg		bc2Cfg;

extern const McHardwareInterfacesImplementation::PinCfg gpCfg[ 35 ] = {
	buttonIncCfg,					buttonDecCfg,				/// Button.
	br0Cfg,			br1Cfg,			br2Cfg,						/// KeyBoard.
	bc0Cfg,			bc1Cfg,			bc2Cfg,
	adcCfg,														/// ADC.
	lcdClkCfg,		lcdPwmCfg,		lcdResCfg,					/// LCD.
	lcdDcCfg,		lcdDcCfg,		lcdCsCfg,
	sd1SmdCfg,		sdIoAndClkCfg,	sdCheckerValueSetter,		/// MicroSd.
	sdPushCfg,		sdPushCfg,
	usbCfg,														/// USB.
	pwr5vOnCfg,		pwrOnCfg,		tpStCfg,					/// PWR.
	tpChCfg,
	uartDebugCfg,												/// Debug UART.
	jtagPart1Cfg,	jtagPart1Cfg,								/// JTAG.
	spiBoardTxCfg,	spiBoardClkCfg,								/// Board.
	boardGpio1,		boardGpio2,		boardGpio3,
	boardGpio4
};



