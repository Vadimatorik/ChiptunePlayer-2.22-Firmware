#include "adc_one_ch.h"

#ifdef HAL_ADC_MODULE_ENABLED

namespace mc {

AdcOneChannel::AdcOneChannel (const AdcOneChannelCfg *const cfg, const uint32_t countCfg) :
    cfg(cfg), countCfg(countCfg) {}

mc_interfaces::res AdcOneChannel::reinit (uint32_t numberCfg) {
    if (numberCfg >= this->countCfg) return mc_interfaces::res::err_input_value;
    
    /// Заполняем HAL-структуру.
    this->adc.Instance = this->cfg[numberCfg].ADCx;

#if defined( STM32F2 ) || defined( STM32F4 )
    this->adc.Init.ClockPrescaler = this->cfg[numberCfg].clockPrescaler;
    this->adc.Init.Resolution = this->cfg[numberCfg].resolution;
#endif
    this->adc.Init.DataAlign = this->cfg[numberCfg].dataAlign;
    this->adc.Init.ScanConvMode = DISABLE;
    this->adc.Init.ContinuousConvMode = ENABLE;
    this->adc.Init.DiscontinuousConvMode = DISABLE;
#if defined( STM32F2 ) || defined( STM32F4 )
    this->adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    this->adc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
#endif
    this->adc.Init.NbrOfConversion = 1;
#if defined( STM32F2 ) || defined( STM32F4 )
    this->adc.Init.DMAContinuousRequests = DISABLE;
    this->adc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
#endif
    
    this->channelCfg.Channel = this->cfg[numberCfg].channel;
    this->channelCfg.Rank = 1;
    this->channelCfg.SamplingTime = this->cfg[numberCfg].samplingTime;
    
    this->clkDisable();
    this->clkEnable();
    
    HAL_ADC_DeInit(&this->adc);
    
    HAL_StatusTypeDef r;
    r = HAL_ADC_Init(&this->adc);
    if (r != HAL_OK) return mc_interfaces::res::err_init;
    
    r = HAL_ADC_ConfigChannel(&this->adc, &this->channelCfg);
    if (r != HAL_OK) return mc_interfaces::res::err_init;
    
    return mc_interfaces::res::err_ok;
}

void AdcOneChannel::clkEnable (void) {
    switch ((uint32_t)this->adc.Instance) {
        case ADC1_BASE:
            __HAL_RCC_ADC1_CLK_ENABLE();
            break;
        case ADC2_BASE:
            __HAL_RCC_ADC2_CLK_ENABLE();
            break;
#ifdef ADC3_BASE
        case ADC3_BASE:
            __HAL_RCC_ADC3_CLK_ENABLE();
            break;
#endif
    }
}

void AdcOneChannel::clkDisable (void) {
    switch ((uint32_t)this->adc.Instance) {
        case ADC1_BASE:
            __HAL_RCC_ADC1_CLK_DISABLE();
            break;
        case ADC2_BASE:
            __HAL_RCC_ADC2_CLK_DISABLE();
            break;
#ifdef ADC3_BASE
        case ADC3_BASE:
            __HAL_RCC_ADC3_CLK_DISABLE();
            break;
#endif
    }
}

mc_interfaces::res AdcOneChannel::startContinuousConversion (void) {
    HAL_StatusTypeDef r;
    r = HAL_ADC_Start(&this->adc);
    if (r != HAL_OK) return mc_interfaces::res::err_init;
    return mc_interfaces::res::err_ok;
}

void AdcOneChannel::stopContinuousConversion (void) {
    HAL_ADC_Stop(&this->adc);
}

uint32_t AdcOneChannel::getMeasurement (void) {
    return this->adc.Instance->DR;
}

void AdcOneChannel::irq_handler (void) {
    this->adc.Instance->SR = 0;
}

}

#endif
