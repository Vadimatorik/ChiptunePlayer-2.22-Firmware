
#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_DMA_MODULE_ENABLED

namespace mc {

#if defined( STM32F2 ) || defined( STM32F4 )

void dmaClkOn (DMA_Stream_TypeDef *dma);

void dmaIrqOn (DMA_Stream_TypeDef *dma, uint32_t prio);

#endif

#if defined( STM32F1 )
void dmaClkOn ( DMA_Channel_TypeDef* dma);
void dmaIrqOn ( DMA_Channel_TypeDef* dma, uint32_t prio );
#endif

}

#endif

#endif
