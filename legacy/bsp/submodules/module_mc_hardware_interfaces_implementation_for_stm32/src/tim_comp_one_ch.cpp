#include "tim_comp_one_ch.h"

#ifdef HAL_TIM_MODULE_ENABLED

namespace mc {

extern void clkTimInit (TIM_TypeDef *tim);

TimCompOneChannel::TimCompOneChannel (const TimCompOneChannelCfg *const cfg) : cfg(cfg) {
    this->tim.Instance = this->cfg->tim;
    
    this->tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    this->tim.Init.CounterMode = TIM_COUNTERMODE_UP;
    
    this->timCh.OCMode = TIM_OCMODE_TOGGLE;
    this->timCh.OCPolarity = this->cfg->polarity;
    this->timCh.Pulse = 0;
}

mc_interfaces::res TimCompOneChannel::reinit (uint32_t numberCfg) {
    if (numberCfg >= this->cfg->countCfg)
        return mc_interfaces::res::err_input_value;
    
    this->tim.Init.Period = this->cfg->cfg[numberCfg].period;
    this->tim.Init.Prescaler = this->cfg->cfg[numberCfg].prescaler;
    this->timCh.Pulse = this->cfg->cfg[numberCfg].pulse;
    
    clkTimInit(this->tim.Instance);
    
    if (HAL_TIM_OC_DeInit(&this->tim) != HAL_OK)
        return mc_interfaces::res::err_init;
    
    if (HAL_TIM_OC_Init(&this->tim) != HAL_OK)
        return mc_interfaces::res::err_init;
    
    if (HAL_TIM_OC_ConfigChannel(&this->tim, &this->timCh, this->cfg->outChannel) != HAL_OK)
        return mc_interfaces::res::err_init;
    
    return mc_interfaces::res::err_ok;
}

mc_interfaces::res TimCompOneChannel::on (void) {
    if (this->tim.State == HAL_TIM_STATE_RESET)
        return mc_interfaces::res::err_init;
    
    HAL_TIM_OC_Start(&this->tim, this->cfg->outChannel);
    HAL_TIMEx_OCN_Start(&this->tim, this->cfg->outChannel);
    
    return mc_interfaces::res::err_ok;
}

void TimCompOneChannel::off (void) {
    this->tim.Instance->CR1 &= ~(TIM_CR1_CEN);
}

}

#endif

