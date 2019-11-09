#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_UART_MODULE_ENABLED

#include "dma.h"
#include "pin.h"
#include "mc_uart.h"
#include "user_os.h"

namespace mc {

struct uart_cfg {
    USART_TypeDef *uart;
    Pin *de;
    uint32_t baudrate;
    uint32_t mode;                        // UART_MODE_RX/UART_MODE_TX/UART_MODE_TX_RX.
#if defined( STM32F2 ) || defined( STM32F4 )
    DMA_Stream_TypeDef *dmaTx;                        // Из мерии DMAx_Streamx.
    uint32_t dmaTxCh;                    // Из серии DMA_CHANNEL_x.
#elif defined( STM32F1 )
    DMA_Channel_TypeDef*		dmaTx;
#endif
    uint32_t dmaTxPrio;                    // Приоритет прерывания.
    void (*byte_handler) (uint8_t byte);
};

class uart_base : public mc_interfaces::uart {
public:
    uart_base (const uart_cfg *const cfg, uint32_t cfg_num = 1);

public:
    mc_interfaces::res reinit (uint32_t cfgNumber = 0);

public:
    mc_interfaces::res on (void);
    void off (void);

public:
    mc_interfaces::res tx (const uint8_t *const buf, uint16_t len = 1, uint32_t timeout_ms = 100);
    mc_interfaces::res get_byte (uint8_t *buf);
    mc_interfaces::res get_byte (uint8_t *buf, uint32_t timeout_ms = 100);

public:
    virtual void uart_irq_handler (void) = 0;
    void dma_irq_handler (void);

public:
    void give_semaphore (void);

private:
    bool clkInit (void);
    bool clkDeinit (void);

protected:
    const uart_cfg *const cfg = nullptr;
    const uint32_t cfg_num = 0;
    
    DMA_HandleTypeDef d;
    UART_HandleTypeDef u;
    SemaphoreHandle_t m = nullptr;
    SemaphoreHandle_t s = nullptr;

    StaticSemaphore_t sb;
    StaticSemaphore_t mb;
};

}

#endif

#endif
