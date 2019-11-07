#include "stm32f4xx_hal.h"

#include <stdio.h>

static char stdout_buffer[1024] = {0};

int init_core () {
    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();


    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, stdout_buffer, _IOFBF, sizeof(stdout_buffer));

    return 0;
}