#pragma once

#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_adc_one_channel.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_pin.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_pin_multifunc_it.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_port_global.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_pwr.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_rcc.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_spi_master_8bit.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_timer_counter.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_timer_interrupt.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_timer_pwm_one_channel.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_uart.h"
#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_wdt.h"
#include "user_os.h"

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
extern McHardwareInterfacesImplementation::Pin					sdSet;
extern McHardwareInterfacesImplementation::PinMultifuncIt		sdRead;
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
