/*

@startuml

namespace mc {

mc.TimCounter ..|> mc.TimCounter

class TimCounter {
	{field}-	const ClkTimBaseCfg*		const cfg
	{field}-	TIM_HandleTypeDef		tim

	__Constructor__
	{method}+	TimCounter	( const ClkTimBaseCfg*	const cfg )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_TIM_MODULE_ENABLED

#include "mc_tim_cnt.h"

namespace mc {

struct ClkTimBaseCfg {
    const uint32_t period;                    // 0..0xFFFF или 0..0xFFFFFFFF
    const uint32_t prescaler;                // 0..0xFFFF или 0..0xFFFFFFFF.
    
    /// Используется только в:
    /// 1. timCompOneChannelCfg
    const uint16_t pulse;                    // 0..0xFFFF или 0..0xFFFFFFFF.
};

struct TimCounterCfg {
    // Используемый таймер.
    TIM_TypeDef *tim;
    
    const ClkTimBaseCfg *const cfg;
    uint32_t countCfg;
};

class TimCounter : public mc_interfaces::TimCounter {
public:
    TimCounter (const TimCounterCfg *const cfg);

    mc_interfaces::res reinit (uint32_t cfgNumber = 0);

    mc_interfaces::res on (void);
    
    void off (void);
    
    uint32_t getCounter (void);

private:
    const TimCounterCfg *const cfg;
    
    TIM_HandleTypeDef tim;
};

}

#endif

#endif
