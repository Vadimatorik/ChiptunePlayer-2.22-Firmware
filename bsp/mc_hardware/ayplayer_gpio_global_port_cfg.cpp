#include "pin.h"
#include "port.h"

extern const pinCfg		adcCfg;
extern const pinCfg		buttonIncCfg;
extern const pinCfg		buttonDecCfg;
extern const pinCfg		lcdClkCfg;
extern const pinCfg		lcdPwmCfg;
extern const pinCfg		lcdResCfg;
extern const pinCfg		lcdDcCfg;
extern const pinCfg		lcdCsCfg;
extern const pinCfg		sd1SmdCfg;
extern const pinCfg		sdIoAndClkCfg;
extern const pinCfg		sdPushCfg;
extern const pinCfg		sdCheckerValueSetter;
extern const pinCfg		usbCfg;
extern const pinCfg		spiBoardTxCfg;
extern const pinCfg		spiBoardClkCfg;
extern const pinCfg		boardGpio1;
extern const pinCfg		boardGpio2;
extern const pinCfg		boardGpio3;
extern const pinCfg		boardGpio4;
extern const pinCfg		jtagPart1Cfg;
extern const pinCfg		jtagPart1Cfg;
extern const pinCfg		pwr5vOnCfg;
extern const pinCfg		pwrOnCfg;
extern const pinCfg		tpStCfg;
extern const pinCfg		tpChCfg;
extern const pinCfg		uartDebugCfg;
extern const pinCfg		br0Cfg;
extern const pinCfg		br1Cfg;
extern const pinCfg		br2Cfg;
extern const pinCfg		bc0Cfg;
extern const pinCfg		bc1Cfg;
extern const pinCfg		bc2Cfg;

extern const pinCfg gpCfg[ 35 ] = {
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



