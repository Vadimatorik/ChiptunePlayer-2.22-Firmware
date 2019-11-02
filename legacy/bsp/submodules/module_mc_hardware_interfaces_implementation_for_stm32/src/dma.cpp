#include "dma.h"

#ifdef HAL_DMA_MODULE_ENABLED

namespace mc {

#if defined(STM32F2) || defined(STM32F4)

void dmaClkOn (DMA_Stream_TypeDef *dma) {
    switch ((uint32_t)dma) {
        case DMA1_Stream0_BASE:
        case DMA1_Stream1_BASE:
        case DMA1_Stream2_BASE:
        case DMA1_Stream3_BASE:
        case DMA1_Stream4_BASE:
        case DMA1_Stream5_BASE:
        case DMA1_Stream6_BASE:
        case DMA1_Stream7_BASE:
            __HAL_RCC_DMA1_CLK_ENABLE();
            break;
    };
    
    switch ((uint32_t)dma) {
        case DMA2_Stream0_BASE:
        case DMA2_Stream1_BASE:
        case DMA2_Stream2_BASE:
        case DMA2_Stream3_BASE:
        case DMA2_Stream4_BASE:
        case DMA2_Stream5_BASE:
        case DMA2_Stream6_BASE:
        case DMA2_Stream7_BASE:
            __HAL_RCC_DMA2_CLK_ENABLE();
            break;
    };
}

void dmaIrqOn (DMA_Stream_TypeDef *dma, uint32_t prio) {
    switch ((uint32_t)dma) {
        case DMA1_Stream0_BASE:
            NVIC_SetPriority(DMA1_Stream0_IRQn, prio);
            NVIC_EnableIRQ(DMA1_Stream0_IRQn);
            break;
        case DMA1_Stream1_BASE:
            NVIC_SetPriority(DMA1_Stream1_IRQn, prio);
            NVIC_EnableIRQ(DMA1_Stream1_IRQn);
            break;
        case DMA1_Stream2_BASE:
            NVIC_SetPriority(DMA1_Stream2_IRQn, prio);
            NVIC_EnableIRQ(DMA1_Stream2_IRQn);
            break;
        case DMA1_Stream3_BASE:
            NVIC_SetPriority(DMA1_Stream3_IRQn, prio);
            NVIC_EnableIRQ(DMA1_Stream3_IRQn);
            break;
        case DMA1_Stream4_BASE:
            NVIC_SetPriority(DMA1_Stream4_IRQn, prio);
            NVIC_EnableIRQ(DMA1_Stream4_IRQn);
            break;
        case DMA1_Stream5_BASE:
            NVIC_SetPriority(DMA1_Stream5_IRQn, prio);
            NVIC_EnableIRQ(DMA1_Stream5_IRQn);
            break;
        case DMA1_Stream6_BASE:
            NVIC_SetPriority(DMA1_Stream6_IRQn, prio);
            NVIC_EnableIRQ(DMA1_Stream6_IRQn);
            break;
        case DMA1_Stream7_BASE:
            NVIC_SetPriority(DMA1_Stream7_IRQn, prio);
            NVIC_EnableIRQ(DMA1_Stream7_IRQn);
            break;
    };
    
    switch ((uint32_t)dma) {
        case DMA2_Stream0_BASE:
            NVIC_SetPriority(DMA2_Stream0_IRQn, prio);
            NVIC_EnableIRQ(DMA2_Stream0_IRQn);
            break;
        case DMA2_Stream1_BASE:
            NVIC_SetPriority(DMA2_Stream1_IRQn, prio);
            NVIC_EnableIRQ(DMA2_Stream1_IRQn);
            break;
        case DMA2_Stream2_BASE:
            NVIC_SetPriority(DMA2_Stream2_IRQn, prio);
            NVIC_EnableIRQ(DMA2_Stream2_IRQn);
            break;
        case DMA2_Stream3_BASE:
            NVIC_SetPriority(DMA2_Stream3_IRQn, prio);
            NVIC_EnableIRQ(DMA2_Stream3_IRQn);
            break;
        case DMA2_Stream4_BASE:
            NVIC_SetPriority(DMA2_Stream4_IRQn, prio);
            NVIC_EnableIRQ(DMA2_Stream4_IRQn);
            break;
        case DMA2_Stream5_BASE:
            NVIC_SetPriority(DMA2_Stream5_IRQn, prio);
            NVIC_EnableIRQ(DMA2_Stream5_IRQn);
            break;
        case DMA2_Stream6_BASE:
            NVIC_SetPriority(DMA2_Stream6_IRQn, prio);
            NVIC_EnableIRQ(DMA2_Stream6_IRQn);
            break;
        case DMA2_Stream7_BASE:
            NVIC_SetPriority(DMA2_Stream7_IRQn, prio);
            NVIC_EnableIRQ(DMA2_Stream7_IRQn);
            break;
    };
}

#endif

#if defined( STM32F1 )

void dmaClkOn ( DMA_Channel_TypeDef* dma) {
#ifdef DMA1
    switch ( (uint32_t)dma ) {
    case DMA1_Channel1_BASE:
    case DMA1_Channel2_BASE:
    case DMA1_Channel3_BASE:
    case DMA1_Channel4_BASE:
    case DMA1_Channel5_BASE:
    case DMA1_Channel6_BASE:
    case DMA1_Channel7_BASE:
        __HAL_RCC_DMA1_CLK_ENABLE();
        break;
    };
#endif

#ifdef DMA2
    switch ( (uint32_t)dma ) {
    case DMA2_Channel1_BASE:
    case DMA2_Channel2_BASE:
    case DMA2_Channel3_BASE:
    case DMA2_Channel4_BASE:
    case DMA2_Channel5_BASE:
    case DMA2_Channel6_BASE:
    case DMA2_Channel7_BASE:
        __HAL_RCC_DMA2_CLK_ENABLE();
        break;
    };
#endif
}

void dmaIrqOn ( DMA_Channel_TypeDef* dma, uint32_t prio ) {
#ifdef DMA1
    switch ( (uint32_t)dma ) {
    case DMA1_Channel1_BASE: NVIC_SetPriority( DMA1_Channel1_IRQn, prio );	NVIC_EnableIRQ( DMA1_Channel1_IRQn ); break;
    case DMA1_Channel2_BASE: NVIC_SetPriority( DMA1_Channel2_IRQn, prio );	NVIC_EnableIRQ( DMA1_Channel2_IRQn ); break;
    case DMA1_Channel3_BASE: NVIC_SetPriority( DMA1_Channel3_IRQn, prio );	NVIC_EnableIRQ( DMA1_Channel3_IRQn ); break;
    case DMA1_Channel4_BASE: NVIC_SetPriority( DMA1_Channel4_IRQn, prio );	NVIC_EnableIRQ( DMA1_Channel4_IRQn ); break;
    case DMA1_Channel5_BASE: NVIC_SetPriority( DMA1_Channel5_IRQn, prio );	NVIC_EnableIRQ( DMA1_Channel5_IRQn ); break;
    case DMA1_Channel6_BASE: NVIC_SetPriority( DMA1_Channel6_IRQn, prio );	NVIC_EnableIRQ( DMA1_Channel6_IRQn ); break;
    case DMA1_Channel7_BASE: NVIC_SetPriority( DMA1_Channel7_IRQn, prio );	NVIC_EnableIRQ( DMA1_Channel7_IRQn ); break;
    };
#endif

#ifdef DMA2
    switch ( (uint32_t)dma ) {
    case DMA2_Channel1_BASE: NVIC_SetPriority( DMA2_Channel1_IRQn, prio );	NVIC_EnableIRQ( DMA2_Channel1_IRQn ); break;
    case DMA2_Channel2_BASE: NVIC_SetPriority( DMA2_Channel2_IRQn, prio );	NVIC_EnableIRQ( DMA2_Channel2_IRQn ); break;
    case DMA2_Channel3_BASE: NVIC_SetPriority( DMA2_Channel3_IRQn, prio );	NVIC_EnableIRQ( DMA2_Channel3_IRQn ); break;
    case DMA2_Channel4_BASE: NVIC_SetPriority( DMA2_Channel4_IRQn, prio );	NVIC_EnableIRQ( DMA2_Channel4_IRQn ); break;
    case DMA2_Channel5_BASE: NVIC_SetPriority( DMA2_Channel5_IRQn, prio );	NVIC_EnableIRQ( DMA2_Channel5_IRQn ); break;
    case DMA2_Channel6_BASE: NVIC_SetPriority( DMA2_Channel6_IRQn, prio );	NVIC_EnableIRQ( DMA2_Channel6_IRQn ); break;
    case DMA2_Channel7_BASE: NVIC_SetPriority( DMA2_Channel7_IRQn, prio );	NVIC_EnableIRQ( DMA2_Channel7_IRQn ); break;
    };
#endif

}

#endif

}

#endif
