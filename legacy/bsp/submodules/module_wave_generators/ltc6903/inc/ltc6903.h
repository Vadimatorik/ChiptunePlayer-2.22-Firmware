#pragma once

#include "project_config.h"

#ifdef MODULE_WAVE_GENERATORS_LTC6903

#include "mc_pin.h"
#include "mc_spi_master_8bit.h"
#include "user_os.h"
#include <errno.h>
#include <math.h>

namespace WaveGenerators {

struct LTC6903Cfg {
    mc_interfaces::Pin *const cs;
    mc_interfaces::SpiMaster8Bit *const s;
};

enum class LTC6903OutputMode {
    CLK_ON_INV_ON_PLUS_180 = 0,
    CLK_OFF_INV_ON = 1,
    CLK_ON_INV_OFF = 2,
    CLK_OFF_INV_OFF = 3
};

class LTC6903 {
public:
    LTC6903 (const LTC6903Cfg *const cfg);
    
    int init (void);
    
    int setFrequency (float frequency,
                      LTC6903OutputMode mode);

private:
    const LTC6903Cfg *const cfg;
    
    USER_OS_STATIC_MUTEX_BUFFER mb;
    USER_OS_STATIC_MUTEX m = nullptr;
};

}

#endif

