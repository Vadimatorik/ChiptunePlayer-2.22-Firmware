#include "stm32f4xx_hal.h"

#include <stdio.h>

char stdin_buffer[1024];
char stdout_buffer[1024];
char stderr_buffer[1024];

int init_core () {
    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    setvbuf(stdin, stdin_buffer, _IOLBF, sizeof(stdin_buffer));
    setvbuf(stdout, stdout_buffer, _IOLBF, sizeof(stdout_buffer));
    setvbuf(stderr, stderr_buffer, _IOLBF, sizeof(stderr_buffer));

    return 0;
}