/*

@startuml

namespace mc {

mc.TimPwmOneChannel ..|> mc.TimPwmOneChannel

class TimPwmOneChannel {
	{field}-	const TimPwmOneChannelCfg*		const cfg
	{field}-	TIM_HandleTypeDef				tim
	{field}-	TIM_OC_InitTypeDef				timChannel

	__Constructor__
	{method}+	TimPwmOneChannel	( const TimPwmOneChannelCfg*	const cfg )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_TIM_MODULE_ENABLED

#include "mc_tim_pwm_one_ch.h"
#include "tim_cnt.h"

namespace mc {

struct TimPwmOneChannelCfg {
    // Используемый таймер.
    TIM_TypeDef *tim;
    
    const ClkTimBaseCfg *const cfg;
    uint32_t countCfg;
    
    // Параметры выходного канала.
    uint8_t outChannel;            // TIM_CHANNEL_x.
    uint32_t polarity;            // TIM_OCPOLARITY_LOW  / TIM_OCPOLARITY_HIGH.
};

class TimPwmOneChannel : public mc_interfaces::TimPwmOneChannel {
public:
    TimPwmOneChannel (const TimPwmOneChannelCfg *const cfg);

    mc_interfaces::res reinit (uint32_t cfgNumber = 0);
    mc_interfaces::res on (void);
    
    void off (void);
    
    void setDuty (float duty);

private:
    const TimPwmOneChannelCfg *const cfg;
    
    TIM_HandleTypeDef tim;
    TIM_OC_InitTypeDef timChannel;
};

}

#endif

#endif
