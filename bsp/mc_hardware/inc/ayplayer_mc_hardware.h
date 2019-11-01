#pragma once

#include "adc_one_ch.h"
#include "pin.h"
#include "pin_multifunc_it.h"
#include "port_global.h"
#include "pwr.h"
#include "clk.h"
#include "spi_master_8bit.h"
#include "tim_cnt.h"
#include "tim_int.h"
#include "tim_pwm_one_ch.h"
#include "uart.h"
#include "wdt.h"
#include "user_os.h"
#include "microsd_card_sdio.h"

extern mc::SpiMaster8Bit boardSpi;
extern mc::Wdt wdt;
extern mc::uart usart;
extern mc::TimInterrupt interruptAy;
extern mc::TimPwmOneChannel lcdPwm;
extern mc::SpiMaster8Bit lcdSpi;
extern mc::SpiMaster8Bit boardSpi;
extern mc::Rcc rcc;
extern mc::Pwr pwr;
extern mc::AdcOneChannel adcBat;
extern MicrosdSdio sd;

extern mc::Pin shStrob;
extern mc::PinMultifuncIt buttonInc;
extern mc::PinMultifuncIt buttonDec;
extern mc::Pin lcdRes;
extern mc::Pin lcdDc;
extern mc::Pin lcdCs;
extern mc::Pin sdSet;
extern mc::PinMultifuncIt sdRead;
extern mc::Pin shStrob;
extern mc::Pin ad5204Cs;
extern mc::Pin ltcCs;
extern mc::Pin pwr5vOn;
extern mc::Pin pwrOn;
extern mc::Pin tpSt;
extern mc::Pin tpCh;
extern mc::Pin keyboardStrings[3];
extern mc::Pin keyboardColumns[3];

extern mc::PortGlobal gp;

#ifdef configGENERATE_RUN_TIME_STATS
extern mc::TimCounter timRunTimeStats;
#endif
