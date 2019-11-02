#include "uart.h"

#ifdef HAL_UART_MODULE_ENABLED

namespace mc {

void uart::uart_irq_handler (void) {
    if (this->cfg->byte_handler) {
        if (__HAL_UART_GET_FLAG(&this->u, UART_FLAG_RXNE)) {
            uint8_t data = this->u.Instance->DR;
            this->cfg->byte_handler(data);
        }
    }

    HAL_UART_IRQHandler(&this->u);
}

}

#endif
