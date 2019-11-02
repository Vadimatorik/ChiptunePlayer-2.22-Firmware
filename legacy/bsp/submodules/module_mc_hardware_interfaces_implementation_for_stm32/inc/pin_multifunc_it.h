/*

@startuml

namespace mc {

mc.PinMultifuncIt --|> mc.PinMultifunc
mc.PinMultifuncIt ..|> mc.PinMultifuncIt

class PinMultifuncIt {
	{field}-	const uint32_t			exitPin
	__Constructor__
	{method}+	PinMultifuncIt\t( const PinCfg*\t\tconst cfg,\n\t\t\t  uint32_t\t\t\tcfgCount,\n\t\t\t  uint32_t\t\t\texitPin )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#include "pin_multifunc.h"
#include "mc_pin_multifunc_it.h"

namespace mc {

class PinMultifuncIt : public mc::PinMultifunc, public mc_interfaces::PinMultifuncIt {
public:
    /// exitPin - GPIO_PIN_x.
    PinMultifuncIt (const PinCfg *const cfg,
                    uint32_t cfgCount,
                    uint32_t exitPin)
        : mc::PinMultifunc(cfg, cfgCount), exitPin(exitPin) {}
    
    bool checkIt (void);
    
    void clearIt (void);

private:
    const uint32_t exitPin;
    
};

}

#endif

#endif
