#include "../../core/inc/FreeRTOS.h"
#include <string.h>

void *malloc (size_t size) {
    return pvPortMalloc(size);
}

void free (void *ptr) {
    return vPortFree(ptr);
}

int vPortGetSizeBlock (void *pv);

void *realloc (void *ptr, size_t new_size) {
    if (ptr == NULL) {
        return malloc(new_size);
    }

    void *p = malloc(new_size);
    if (p == NULL) {
        return p;
    }

    size_t old_size = vPortGetSizeBlock(ptr);
    size_t cpy_len = (new_size < old_size)?new_size:old_size;
    memcpy(p, ptr, cpy_len);
    free(ptr);

    return p;
}

void *l_alloc (void *ud, void *ptr, size_t osize, size_t nsize) {
    (void)ud; (void)osize;  /* not used */
    if (nsize == 0) {
        free(ptr);
        return NULL;
    }
    else
        return realloc(ptr, nsize);
}
