#include "tim_int.h"

#ifdef HAL_TIM_MODULE_ENABLED

namespace mc {

extern void clkTimInit (TIM_TypeDef *tim);

TimInterrupt::TimInterrupt (const TimInterruptCfg *const cfg) : cfg(cfg) {
    this->tim.Instance = this->cfg->tim;
    
    this->tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    this->tim.Init.CounterMode = TIM_COUNTERMODE_UP;
}

mc_interfaces::res TimInterrupt::reinit (uint32_t numberCfg) {
    if (numberCfg >= this->cfg->countCfg)
        return mc_interfaces::res::err_input_value;
    
    this->tim.Init.Period = this->cfg->cfg[numberCfg].period;
    this->tim.Init.Prescaler = this->cfg->cfg[numberCfg].prescaler;
    
    clkTimInit(this->cfg->tim);
    
    if (HAL_TIM_Base_DeInit(&this->tim) != HAL_OK)
        return mc_interfaces::res::err_init;
    
    if (HAL_TIM_Base_Init(&this->tim) != HAL_OK)
        return mc_interfaces::res::err_init;
    
    return mc_interfaces::res::err_ok;
}

mc_interfaces::res TimInterrupt::setState (bool state) {
    if (this->tim.State == HAL_TIM_STATE_RESET)
        return mc_interfaces::res::err_init;
    
    if (state) {
        HAL_TIM_Base_Start_IT(&this->tim);
    } else {
        HAL_TIM_Base_Stop_IT(&this->tim);
    }
    
    return mc_interfaces::res::err_ok;
}

mc_interfaces::res TimInterrupt::on (void) {
    if (this->tim.State == HAL_TIM_STATE_RESET)
        return mc_interfaces::res::err_init;
    
    HAL_TIM_Base_Start_IT(&this->tim);
    
    return mc_interfaces::res::err_ok;
}

void TimInterrupt::off (void) {
    HAL_TIM_Base_Stop_IT(&this->tim);
}

void TimInterrupt::clearInterruptFlag (void) {
    HAL_TIM_IRQHandler(&this->tim);
}

}

#endif
