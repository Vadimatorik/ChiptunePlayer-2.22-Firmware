#include "uart.h"
#include <stdio.h>
#include <string.h>

#include "ayplayer_struct_mcu_cfg.h"
#include "run_time_logger.h"


BaseResult		uartSendMessage ( const char* string );

const runTimeLoggerCfg log_cfg = {
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

BaseResult uartSendMessage ( const char* string ) {
	return usart.tx( (uint8_t*)string, strlen(string), 100 );
}
