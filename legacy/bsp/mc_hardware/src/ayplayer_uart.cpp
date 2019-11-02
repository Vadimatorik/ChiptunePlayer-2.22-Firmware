#include "uart.h"

const mc::uart_cfg usartCfg = {
    .uart                =    USART1,
    .de                    =    nullptr,
    .baudrate    =    115200,
    .mode                =    UART_MODE_TX_RX,
    .dmaTx                =    DMA2_Stream7,
    .dmaTxCh            =    DMA_CHANNEL_4,
    .dmaTxPrio            =    10,
    .byte_handler = nullptr
};

mc::uart usart(&usartCfg);
