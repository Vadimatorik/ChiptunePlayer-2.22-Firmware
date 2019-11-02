/*

@startuml

namespace mc {

mc.TimInterrupt ..|> mc.TimInterrupt

class TimInterrupt {
	{field}-	const TimInterruptCfg*			const cfg
	{field}-	TIM_HandleTypeDef				tim
	__Constructor__
	{method}+	TimInterrupt	( const TimInterruptCfg*	const cfg )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_TIM_MODULE_ENABLED

#include "tim_cnt.h"
#include "mc_tim_int.h"

namespace mc {

struct TimInterruptCfg {
    TIM_TypeDef *tim;
    const ClkTimBaseCfg *const cfg;
    uint32_t countCfg;
};

class TimInterrupt : public mc_interfaces::TimInterrupt {
public:
    TimInterrupt (const TimInterruptCfg *const cfg);
    
    mc_interfaces::res reinit (uint32_t cfgNumber = 0);
    
    mc_interfaces::res setState (bool state);
    
    mc_interfaces::res on (void);
    
    void off (void);
    
    void clearInterruptFlag (void);

private:
    const TimInterruptCfg *const cfg;
    
    TIM_HandleTypeDef tim;
};

}

#endif

#endif
