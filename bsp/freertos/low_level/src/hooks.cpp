#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include <stdint.h>

extern "C" {

void vApplicationMallocFailedHook (void) {
    while (1);
}

void vApplicationStackOverflowHook (void) {
    while (1);
}

}
