#ifdef HARD
#include "stm32f4xx_hal.h"
#endif

#include <stdio.h>

#include "freertos_headers.h"

uint8_t __attribute__ ((aligned (16))) freertos_heap_block_1[1024*106] = {0};

__attribute__ ((section (".bss_ccm")))
uint8_t __attribute__ ((aligned (16))) freertos_heap_block_2[1024*64] = {0};

const HeapRegion_t heap_regions[] = {
    {(uint8_t *)freertos_heap_block_1, sizeof(freertos_heap_block_1)},
    {(uint8_t *)freertos_heap_block_2, sizeof(freertos_heap_block_2)},
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