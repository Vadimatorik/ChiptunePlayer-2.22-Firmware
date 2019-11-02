#include "stm32f4xx_hal.h"

void assert_failed (uint8_t *file, uint32_t line) {
    (void)file;
    (void)line;
    NVIC_SystemReset();
}