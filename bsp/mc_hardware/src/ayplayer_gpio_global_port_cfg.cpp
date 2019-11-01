#include "pin.h"
#include "port_global.h"

extern const mc::PinCfg		adcCfg;
extern const mc::PinCfg		buttonIncCfg;
extern const mc::PinCfg		buttonDecCfg;
extern const mc::PinCfg		lcdClkCfg;
extern const mc::PinCfg		lcdPwmCfg;
extern const mc::PinCfg		lcdResCfg;
extern const mc::PinCfg		lcdDcCfg;
extern const mc::PinCfg		lcdCsCfg;
extern const mc::PinCfg		sd1SmdCfg;
extern const mc::PinCfg		sdIoAndClkCfg;
extern const mc::PinCfg		sdSetCfg;
extern const mc::PinCfg		sdReadCfg;
extern const mc::PinCfg		usbCfg;
extern const mc::PinCfg		spiBoardTxCfg;
extern const mc::PinCfg		spiBoardClkCfg;
extern const mc::PinCfg		boardGpio1;
extern const mc::PinCfg		boardGpio2;
extern const mc::PinCfg		boardGpio3;
extern const mc::PinCfg		boardGpio4;
extern const mc::PinCfg		jtagPart1Cfg;
extern const mc::PinCfg		jtagPart1Cfg;
extern const mc::PinCfg		pwr5vOnCfg;
extern const mc::PinCfg		pwrOnCfg;
extern const mc::PinCfg		tpStCfg;
extern const mc::PinCfg		tpChCfg;
extern const mc::PinCfg		uartDebugCfg;
extern const mc::PinCfg		br0Cfg;
extern const mc::PinCfg		br1Cfg;
extern const mc::PinCfg		br2Cfg;
extern const mc::PinCfg		bc0Cfg;
extern const mc::PinCfg		bc1Cfg;
extern const mc::PinCfg		bc2Cfg;

extern const mc::PinCfg gpCfg[ 35 ] = {
	buttonIncCfg,					buttonDecCfg,				/// Button.
	br0Cfg,			br1Cfg,			br2Cfg,						/// KeyBoard.
	bc0Cfg,			bc1Cfg,			bc2Cfg,
	adcCfg,														/// ADC.
	lcdClkCfg,		lcdPwmCfg,		lcdResCfg,					/// LCD.
	lcdDcCfg,		lcdDcCfg,		lcdCsCfg,
	sd1SmdCfg,		sdIoAndClkCfg,								/// MicroSd.
	sdSetCfg,		sdReadCfg,
	usbCfg,														/// USB.
	pwr5vOnCfg,		pwrOnCfg,		tpStCfg,					/// PWR.
	tpChCfg,
	uartDebugCfg,												/// Debug UART.
	jtagPart1Cfg,	jtagPart1Cfg,								/// JTAG.
	spiBoardTxCfg,	spiBoardClkCfg,								/// Board.
	boardGpio1,		boardGpio2,		boardGpio3,
	boardGpio4
};



