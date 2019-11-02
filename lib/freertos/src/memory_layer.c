#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include <stdint.h>

void *malloc (size_t size) {
    return pvPortMalloc(size);
}

void free (void *ptr) {
    return vPortFree(ptr);
}
