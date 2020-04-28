#ifdef HARD
#include "stm32f4xx_hal.h"
#endif

#include "freertos_headers.h"

__attribute__ ((aligned (16), section (".sdram")))
uint8_t freertos_heap[1024*1024*64] = {0};

const HeapRegion_t heap_regions[] = {
    {(uint8_t *)freertos_heap, sizeof(freertos_heap)},
    {NULL, 0}
};

int init_core () {
#ifdef HARD
    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
#endif

    vPortDefineHeapRegions(heap_regions);

    return 0;
}