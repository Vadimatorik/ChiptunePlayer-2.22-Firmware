#pragma once

#include "user_os.h"

#include "rcc.h"
#include "pwr.h"
#include "wdt.h"
#include "spi.h"
#include "pin.h"
#include "uart.h"
#include "port.h"
#include "timer.h"
#include "adc_one_channel.h"

extern SpiMaster8Bit		boardSpi;
extern Wdt					wdt;
extern Uart					usart;
extern TimInterrupt			interruptAy;
extern TimPwmOneChannel		lcdPwm;
extern SpiMaster8Bit		lcdSpi;
extern SpiMaster8Bit		boardSpi;
extern Rcc					rcc;
extern Pwr					pwr;
extern AdcOneChannel		adcBat;

extern Pin					shStrob;
extern PinMultifuncIt		buttonInc;
extern PinMultifuncIt		buttonDec;
extern Pin 					lcdRes;
extern Pin					lcdDc;
extern Pin					lcdCs;
extern Pin					sdSignal;
extern PinMultifuncIt		sdIn;
extern Pin					shStrob;
extern Pin					ad5204Cs;
extern Pin					ltcCs;
extern Pin					pwr5vOn	;
extern Pin					pwrOn;
extern Pin					tpSt;
extern Pin					tpCh;
extern Pin					keyboardStrings[ 3 ];
extern Pin					keyboardColumns[ 3 ];

extern GlobalPort			gp;

#ifdef configGENERATE_RUN_TIME_STATS
extern TimCounter			timRunTimeStats;
#endif
