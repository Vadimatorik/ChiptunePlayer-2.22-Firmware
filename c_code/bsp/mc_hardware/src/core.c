#ifdef HARD
#include "stm32f4xx_hal.h"
#endif

#include "freertos_headers.h"

__attribute__ ((aligned (16), section (".bss_ram")))
uint8_t freertos_heap_block_0[1024*127] = {0};

const HeapRegion_t heap_regions[] = {
    {(uint8_t *)freertos_heap_block_0, sizeof(freertos_heap_block_0)},
    {NULL,                             0}
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