#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include <stdint.h>

extern "C" {

__attribute__ ((section (".rodata_keep")))
static const uint8_t freeRTOSMemoryScheme = configUSE_HEAP_SCHEME;

void *malloc (size_t size) {
    return pvPortMalloc(size);
}

void *calloc (size_t num, size_t size) {
    return pvPortMalloc(num * size);
}

void free (void *ptr) {
    return vPortFree(ptr);
}

void *operator new (size_t sz) {
    return pvPortMalloc(sz);
}

void *operator new[] (size_t sz) {
    return pvPortMalloc(sz);
}

void operator delete (void *p) {
    vPortFree(p);
}

void operator delete[] (void *p) {
    vPortFree(p);
}

void operator delete (void *p, size_t size) {
    (void)size;
    vPortFree(p);
}

void operator delete[] (void *p, size_t size) {
    (void)size;
    vPortFree(p);
}

}
