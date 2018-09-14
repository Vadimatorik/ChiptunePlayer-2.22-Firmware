#include "../submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc/mc_hardware_interfaces_implementation_for_stm32_uart.h"

const McHardwareInterfacesImplementation::UartCfg usartCfg = {
	.uart				=	USART1,
	.de					=	nullptr,
	.baudrate			=	115200,
	.mode				=	UART_MODE_TX_RX,
	.dmaTx				=	DMA2_Stream7,
	.dmaTxCh			=	DMA_CHANNEL_4,
	.dmaTxPrio			=	10
};

McHardwareInterfacesImplementation::Uart usart( &usartCfg );
