#include <stdint.h>

#include "mc_hardware_interfaces_implementation_for_stm32_timer_counter.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

#ifdef configGENERATE_RUN_TIME_STATS
extern McHardwareInterfacesImplementation::TimCounter			timRunTimeStats;
#endif

extern "C" {

void* malloc(size_t size) {
    return pvPortMalloc(size);
}

void* calloc(size_t num, size_t size) {
    return pvPortMalloc(num * size);
}

void free(void* ptr) {
    return vPortFree(ptr);
}

void* operator new(size_t sz) {
    return pvPortMalloc(sz);
}

void* operator new[](size_t sz) {
    return pvPortMalloc(sz);
}

void operator delete(void* p) {
    vPortFree(p);
}

void operator delete[](void* p) {
    vPortFree(p);
}

void operator delete( void* p, size_t size ) {
	( void )size;
    vPortFree(p);
}

void operator delete[]( void* p, size_t size ) {
	( void )size;
    vPortFree(p);
}

#ifdef configGENERATE_RUN_TIME_STATS

void vConfigureTimerForRunTimeStats ( void ) {
	/// Таймер настроится при конфигурации RCC внутри AyPlayer.
}

uint32_t vGetRunTimeCounterValue ( void ) {
	return timRunTimeStats.getCounter();
}

#endif

}
