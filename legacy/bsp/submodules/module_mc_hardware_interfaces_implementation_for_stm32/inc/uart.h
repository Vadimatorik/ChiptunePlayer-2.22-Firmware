#pragma once

#ifdef __cplusplus

#include "uart_base.h"

#ifdef HAL_UART_MODULE_ENABLED

namespace mc {

class uart : public uart_base {
public:
    uart (const uart_cfg *const cfg, uint32_t cfg_num = 1) :
        uart_base(cfg, cfg_num) {
    }

public:
    void uart_irq_handler ();

};

}

#endif

#endif
