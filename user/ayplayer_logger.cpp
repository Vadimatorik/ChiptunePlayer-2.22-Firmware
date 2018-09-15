#include <stdio.h>
#include <string.h>

#include "../bsp/submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_uart.h"
#include "ayplayer_struct_mcu_cfg.h"
#include "run_time_logger.h"


McHardwareInterfaces::BaseResult		uartSendMessage ( const char* string );

const RunTimeLoggerCfg log_cfg = {
	.color = {
		.initOkColorString			= ANSI_RESET,
		.initIssueColorString		= ANSI_YEL,
		.initErrorColorString		= ANSI_RED,
		.runMessageOkColorString	= ANSI_RESET,
		.runMessageIssueColorString	= ANSI_YEL,
		.runMessageErrorColorString	= ANSI_RED
	},
	.outBuffer = uartSendMessage
};

RunTimeLogger ayLog( &log_cfg );

McHardwareInterfaces::BaseResult uartSendMessage ( const char* string ) {
	return usart.tx( (uint8_t*)string, strlen(string), 100 );
}
