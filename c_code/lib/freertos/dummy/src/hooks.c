#include "../../core/inc/FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include <stdint.h>

void vApplicationMallocFailedHook () {
    while (1);
}

void vApplicationStackOverflowHook () {
    while (1);
}
