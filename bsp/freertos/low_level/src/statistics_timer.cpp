#include "tim_cnt.h"
#include "FreeRTOSConfig.h"
#include <stdint.h>

#ifdef configGENERATE_RUN_TIME_STATS
extern mc::TimCounter timRunTimeStats;
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
