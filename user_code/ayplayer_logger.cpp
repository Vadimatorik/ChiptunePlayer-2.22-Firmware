#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "ayplayer_class/structs/ayplayer_struct_mcu_cfg.h"
#include "run_time_logger.h"


mc_interfaces::res		uartSendMessage ( const char* string );

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

mc_interfaces::res uartSendMessage ( const char* string ) {
	return usart.tx( (uint8_t*)string, strlen(string), 100 );
}
