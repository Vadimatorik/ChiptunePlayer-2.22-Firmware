#include "uart_base.h"

#include <string.h>

#ifdef HAL_UART_MODULE_ENABLED

namespace mc {

uart_base::uart_base (const uart_cfg *const cfg, uint32_t cfg_num) :
    cfg(cfg), cfg_num(cfg_num) {

    memset(&this->d, 0, sizeof(this->d));
    memset(&this->u, 0, sizeof(this->u));

    this->u.Init.HwFlowCtl = UART_HWCONTROL_NONE;
#ifdef UART_OVERSAMPLING_8
    this->u.Init.OverSampling = UART_OVERSAMPLING_8;
#elif UART_OVERSAMPLING_16
    this->uart.Init.OverSampling						= UART_OVERSAMPLING_16;
#endif
    this->u.Init.Parity = UART_PARITY_NONE;
    this->u.Init.StopBits = UART_STOPBITS_1;
    this->u.Init.WordLength = UART_WORDLENGTH_8B;
    
    if (cfg->dmaTx != nullptr) {
        this->u.hdmatx = &this->d;
        
        this->u.hdmatx->Init.Direction = DMA_MEMORY_TO_PERIPH;
        this->u.hdmatx->Init.PeriphInc = DMA_PINC_DISABLE;
        this->u.hdmatx->Init.MemInc = DMA_MINC_ENABLE;
        this->u.hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        this->u.hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        this->u.hdmatx->Init.Mode = DMA_NORMAL;
        this->u.hdmatx->Init.Priority = DMA_PRIORITY_HIGH;
#if defined(STM32F2) || defined(STM32F4)
        this->u.hdmatx->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
#endif
    }
    
    this->u.obj = this;
    
    this->m = xSemaphoreCreateMutexStatic(&this->mb);
    this->s = xSemaphoreCreateBinaryStatic(&this->sb);
}

mc_interfaces::res uart_base::reinit (uint32_t numberCfg) {

    if (numberCfg >= this->cfg_num)
        return mc_interfaces::res::err_input_value;
    
    this->clkDeinit();
    
    this->u.Instance = cfg[numberCfg].uart;
    this->u.Init.Mode = cfg[numberCfg].mode;
    this->u.Init.BaudRate = cfg[numberCfg].baudrate;
    this->u.hdmatx->Instance = this->cfg[numberCfg].dmaTx;
#if defined(STM32F2) || defined(STM32F4)
    this->u.hdmatx->Init.Channel = this->cfg[numberCfg].dmaTxCh;
#endif
    
    if (cfg[numberCfg].dmaTx != nullptr) {
        this->u.hdmatx = &this->d;
        this->u.hdmatx->Parent = &this->u;
        dmaClkOn(this->cfg->dmaTx);
        
        if (HAL_DMA_Init(&this->d) != HAL_OK)
            return mc_interfaces::res::err_init;
        
        dmaIrqOn(this->cfg->dmaTx, cfg[numberCfg].dmaTxPrio);
    }
    
    this->clkInit();
    
    HAL_StatusTypeDef r;
    
    r = HAL_UART_DeInit(&this->u);
    if (r != HAL_OK)
        return mc_interfaces::res::err_init;
    
    r = HAL_UART_Init(&this->u);
    if (r != HAL_OK)
        return mc_interfaces::res::err_init;
    
    if (this->cfg->de != nullptr) this->cfg->de->reset();
    
    return mc_interfaces::res::err_ok;
}

mc_interfaces::res uart_base::on (void) {
    if (this->u.gState == HAL_UART_STATE_RESET)
        return mc_interfaces::res::err_init;
    
    __HAL_UART_ENABLE(&this->u);
    __HAL_UART_ENABLE_IT(&this->u, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&this->u, UART_IT_PE);
    __HAL_UART_ENABLE_IT(&this->u, UART_IT_ERR);
    return mc_interfaces::res::err_ok;
}

void uart_base::off (void) {
    __HAL_UART_DISABLE(&this->u);
}

mc_interfaces::res uart_base::tx (const uint8_t *const txArray,
                                           uint16_t len,
                                           uint32_t timeout_ms) {
    xSemaphoreTake(this->m, portMAX_DELAY);
    xSemaphoreTake (this->s, 0);
    
    if (this->cfg->de != nullptr) this->cfg->de->set();
    
    if (this->u.hdmatx != nullptr) {                                                // Если передача идет по DMA.
        HAL_UART_Transmit_DMA(&this->u, (uint8_t *)txArray, len);
    } else {                                                                    // Если по прерываниям.
        HAL_UART_Transmit_IT(&this->u, (uint8_t *)txArray, len);
    }
    
    mc_interfaces::res rv = mc_interfaces::res::err_timeout;
    if (xSemaphoreTake (this->s, timeout_ms) == pdTRUE) {
        rv = mc_interfaces::res::err_ok;
    }
    
    if (this->cfg->de != nullptr) this->cfg->de->reset();
    
    xSemaphoreGive(this->m);
    return rv;
}

mc_interfaces::res uart_base::get_byte (uint8_t *buf) {
    if (this->u.gState == HAL_UART_STATE_RESET) {
        return mc_interfaces::res::err_init;
    }
    
    /// Если есть данные.
    if (__HAL_UART_GET_FLAG(&this->u, UART_FLAG_RXNE)) {
        *buf = this->u.Instance->DR;
        return mc_interfaces::res::err_ok;
    }
    
    return mc_interfaces::res::err_not_data;
}

mc_interfaces::res uart_base::get_byte (uint8_t *buf,
                                   uint32_t timeoutMs) {
    if (this->u.gState == HAL_UART_STATE_RESET) {
        return mc_interfaces::res::err_init;
    }
    
    /// Если есть данные.
    if (__HAL_UART_GET_FLAG(&this->u, UART_FLAG_RXNE)) {
        *buf = this->u.Instance->DR;
        return mc_interfaces::res::err_ok;
    }
    
    xSemaphoreTake(this->m, portMAX_DELAY);
    xSemaphoreTake (this->s, 0);
    
    volatile mc_interfaces::res rv = mc_interfaces::res::err_timeout;
    if (xSemaphoreTake (this->s, timeoutMs) == pdTRUE) {
        *buf = this->u.Instance->DR;
        rv = mc_interfaces::res::err_ok;
    }
    
    xSemaphoreGive(this->m);
    
    return rv;
}

void uart_base::dma_irq_handler (void) {
    if (this->u.hdmatx != nullptr) {
        HAL_DMA_IRQHandler(this->u.hdmatx);
    }
}

extern "C" {

void HAL_UART_TxCpltCallback (UART_HandleTypeDef *huart) {
    ((uart_base *)huart->obj)->give_semaphore();
}

void HAL_UART_RxCpltCallback (UART_HandleTypeDef *huart) {
    ((uart_base *)huart->obj)->give_semaphore();
}

}


// Private.
void uart_base::give_semaphore (void) {
    if (this->s) {
        BaseType_t xHigherPriorityTaskWoken = pdTRUE;
        xSemaphoreGiveFromISR (this->s, &xHigherPriorityTaskWoken);
    }
}

bool uart_base::clkInit (void) {
    switch ((uint32_t)this->cfg->uart) {
#ifdef USART1
        case USART1_BASE:
            __HAL_RCC_USART1_CLK_ENABLE();
            return true;
#endif
#ifdef USART2
        case USART2_BASE:
            __HAL_RCC_USART2_CLK_ENABLE();
            return true;
#endif
#ifdef USART3
        case USART3_BASE:
            __HAL_RCC_USART3_CLK_ENABLE();
            return true;
#endif
#ifdef UART4
        case UART4_BASE:
            __HAL_RCC_UART4_CLK_ENABLE();
            return true;
#endif
#ifdef UART5
        case UART5_BASE:
            __HAL_RCC_UART5_CLK_ENABLE();
            return true;
#endif
#ifdef UART6
            case	UART6_BASE:		__HAL_RCC_UART6_CLK_ENABLE();		return true;
#endif
#ifdef UART7
            case	UART7_BASE:		__HAL_RCC_UART7_CLK_ENABLE();		return true;
#endif
#ifdef UART8
            case	UART8_BASE:		__HAL_RCC_UART8_CLK_ENABLE();		return true;
#endif
    };
    return false;
}

bool uart_base::clkDeinit (void) {
    switch ((uint32_t)this->cfg->uart) {
#ifdef USART1
        case USART1_BASE:
            __HAL_RCC_USART1_CLK_DISABLE();
            return true;
#endif
#ifdef USART2
        case USART2_BASE:
            __HAL_RCC_USART2_CLK_DISABLE();
            return true;
#endif
#ifdef USART3
        case USART3_BASE:
            __HAL_RCC_USART3_CLK_DISABLE();
            return true;
#endif
#ifdef UART4
        case UART4_BASE:
            __HAL_RCC_UART4_CLK_DISABLE();
            return true;
#endif
#ifdef UART5
        case UART5_BASE:
            __HAL_RCC_UART5_CLK_DISABLE();
            return true;
#endif
#ifdef UART6
            case	UART6_BASE:		__HAL_RCC_UART6_CLK_DISABLE();		return true;
#endif
#ifdef UART7
            case	UART7_BASE:		__HAL_RCC_UART7_CLK_DISABLE();		return true;
#endif
#ifdef UART8
            case	UART8_BASE:		__HAL_RCC_UART8_CLK_DISABLE();		return true;
#endif
    };
    return false;
}

}

#endif
