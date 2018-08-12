#include "uart.h"

const uartCfg usartCfg = {
	.uart				=	USART1,
	.de					=	nullptr,
	.baudrate			=	115200,
	.mode				=	UART_MODE_TX_RX,
	.dmaTx				=	DMA2_Stream7,
	.dmaTxCh			=	DMA_CHANNEL_4,
};

Uart usart( &usartCfg );
