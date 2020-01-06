#ifdef AYM_HARDWARE
#include "stm32f4xx_hal.h"
#endif

#include <stdio.h>

__attribute__ ((section (".bss_ccm")))
static char stdout_buffer[BUFSIZ] = {0};

__attribute__ ((section (".bss_ccm")))
static char stdin_buffer[BUFSIZ] = {0};

int init_core () {
#ifdef AYM_HARDWARE
    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
#endif

    setbuf(stdout, stdout_buffer);
    setbuf(stdin, stdin_buffer);

    return 0;
}