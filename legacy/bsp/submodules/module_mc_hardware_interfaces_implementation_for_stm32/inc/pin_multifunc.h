/*

@startuml

namespace mc {

mc.PinMultifunc		--|>	mc.Pin
mc.PinMultifunc 	..|> 	mc.PinMultifunc

class PinMultifunc {
	{field}-	const uint32_t		cfgCount
	__Constructor__
	{method}+	PinMultifunc\t( const PinCfg*\tconst cfg,\n\t\t\t  uint32_t\t\tcfgCount )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#include "pin.h"
#include "mc_pin_multifunc.h"

namespace mc {

class PinMultifunc : public mc::Pin, public mc_interfaces::PinMultifunc {
public:
    PinMultifunc (const PinCfg *const cfg,
                  uint32_t cfgCount)
        : Pin(cfg), cfgCount(cfgCount) {}
    
    bool reinit (uint32_t cfgNumber);

protected:
    const uint32_t cfgCount;
};

}

#endif

#endif
