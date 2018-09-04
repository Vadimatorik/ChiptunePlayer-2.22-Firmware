#pragma once

#include "port_global.h"
#include "spi_master_8bit.h"
#include "user_os.h"

#include "rcc.h"
#include "pwr.h"
#include "wdt.h"
#include "pin.h"
#include "pin_multifunc_it.h"
#include "uart.h"
#include "timer_counter.h"
#include "timer_interrupt.h"
#include "timer_pwm_one_channel.h"
#include "adc_one_channel.h"
#include "microsd_card_sdio.h"

extern McHardwareInterfacesImplementation::SpiMaster8Bit		boardSpi;
extern McHardwareInterfacesImplementation::Wdt					wdt;
extern McHardwareInterfacesImplementation::Uart					usart;
extern McHardwareInterfacesImplementation::TimInterrupt			interruptAy;
extern McHardwareInterfacesImplementation::TimPwmOneChannel		lcdPwm;
extern McHardwareInterfacesImplementation::SpiMaster8Bit		lcdSpi;
extern McHardwareInterfacesImplementation::SpiMaster8Bit		boardSpi;
extern McHardwareInterfacesImplementation::Rcc					rcc;
extern McHardwareInterfacesImplementation::Pwr					pwr;
extern McHardwareInterfacesImplementation::AdcOneChannel		adcBat;
extern MicrosdSdio												sd;

extern McHardwareInterfacesImplementation::Pin					shStrob;
extern McHardwareInterfacesImplementation::PinMultifuncIt		buttonInc;
extern McHardwareInterfacesImplementation::PinMultifuncIt		buttonDec;
extern McHardwareInterfacesImplementation::Pin 					lcdRes;
extern McHardwareInterfacesImplementation::Pin					lcdDc;
extern McHardwareInterfacesImplementation::Pin					lcdCs;
extern McHardwareInterfacesImplementation::Pin					sdSignal;
extern McHardwareInterfacesImplementation::PinMultifuncIt		sdIn;
extern McHardwareInterfacesImplementation::Pin					shStrob;
extern McHardwareInterfacesImplementation::Pin					ad5204Cs;
extern McHardwareInterfacesImplementation::Pin					ltcCs;
extern McHardwareInterfacesImplementation::Pin					pwr5vOn	;
extern McHardwareInterfacesImplementation::Pin					pwrOn;
extern McHardwareInterfacesImplementation::Pin					tpSt;
extern McHardwareInterfacesImplementation::Pin					tpCh;
extern McHardwareInterfacesImplementation::Pin					keyboardStrings[ 3 ];
extern McHardwareInterfacesImplementation::Pin					keyboardColumns[ 3 ];

extern McHardwareInterfacesImplementation::PortGlobal			gp;

#ifdef configGENERATE_RUN_TIME_STATS
extern McHardwareInterfacesImplementation::TimCounter			timRunTimeStats;
#endif
