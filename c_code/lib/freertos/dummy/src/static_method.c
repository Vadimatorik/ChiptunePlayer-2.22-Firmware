#include "freertos_headers.h"

__attribute__ ((section (".bss_ccm")))
static StaticTask_t xIdleTaskTCB = {0};

__attribute__ ((section (".bss_ccm")))
static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE] = {0};

void vApplicationGetIdleTaskMemory (StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

__attribute__ ((section (".bss_ccm")))
static StaticTask_t xTimerTaskTCB = {0};

__attribute__ ((section (".bss_ccm")))
static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH] = {0};

void vApplicationGetTimerTaskMemory (StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize) {
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
