#include "pin.h"
#include "port.h"

extern const PinCfg		adcCfg;
extern const PinCfg		buttonIncCfg;
extern const PinCfg		buttonDecCfg;
extern const PinCfg		lcdClkCfg;
extern const PinCfg		lcdPwmCfg;
extern const PinCfg		lcdResCfg;
extern const PinCfg		lcdDcCfg;
extern const PinCfg		lcdCsCfg;
extern const PinCfg		sd1SmdCfg;
extern const PinCfg		sdIoAndClkCfg;
extern const PinCfg		sdPushCfg;
extern const PinCfg		sdCheckerValueSetter;
extern const PinCfg		usbCfg;
extern const PinCfg		spiBoardTxCfg;
extern const PinCfg		spiBoardClkCfg;
extern const PinCfg		boardGpio1;
extern const PinCfg		boardGpio2;
extern const PinCfg		boardGpio3;
extern const PinCfg		boardGpio4;
extern const PinCfg		jtagPart1Cfg;
extern const PinCfg		jtagPart1Cfg;
extern const PinCfg		pwr5vOnCfg;
extern const PinCfg		pwrOnCfg;
extern const PinCfg		tpStCfg;
extern const PinCfg		tpChCfg;
extern const PinCfg		uartDebugCfg;
extern const PinCfg		br0Cfg;
extern const PinCfg		br1Cfg;
extern const PinCfg		br2Cfg;
extern const PinCfg		bc0Cfg;
extern const PinCfg		bc1Cfg;
extern const PinCfg		bc2Cfg;

extern const PinCfg gpCfg[ 35 ] = {
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



