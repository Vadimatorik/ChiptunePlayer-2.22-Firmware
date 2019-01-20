#include "mc_hardware_interfaces_implementation_for_stm32_timer_counter.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include <stdint.h>

#ifdef configGENERATE_RUN_TIME_STATS
extern McHardwareInterfacesImplementation::TimCounter timRunTimeStats;
#endif

extern "C" {

#ifdef configGENERATE_RUN_TIME_STATS

void vConfigureTimerForRunTimeStats (void) {
    /// Таймер настроится при конфигурации RCC внутри AyPlayer.
}

uint32_t vGetRunTimeCounterValue (void) {
    return timRunTimeStats.getCounter();
}

#endif

}
