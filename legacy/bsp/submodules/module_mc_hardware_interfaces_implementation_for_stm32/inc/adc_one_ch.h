/*

@startuml

namespace mc {

mc.AdcOneChannel ..|> mc.AdcOneChannel

class AdcOneChannel {
	{field}-	const AdcOneChannelCfg*		const cfg
	{field}-	const uint32_t				cfgCount
	{field}-	ADC_HandleTypeDef			adc
	{field}-	ADC_ChannelConfTypeDef		channelCfg
	__Constructor__
	{method}+	AdcOneChannel	( const AdcOneChannelCfg*		const cfg,\n\t\t\t  uint32_t\t\t\t\t\tcountCfg )
	__Public methods__
	{method}+	void	irq_handler		( void )
	__Private methods__
	{method}-	void	clkEnable		( void )
	{method}-	void	clkDisable		( void )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_ADC_MODULE_ENABLED

#include "mc_adc_one_channel.h"

namespace mc {

struct AdcOneChannelCfg {
    ADC_TypeDef *ADCx;
    uint32_t clockPrescaler;                                // ADC_ClockPrescaler
    uint32_t resolution;                                    // ADC_Resolution
    uint32_t dataAlign;                                    // ADC_Data_align
    uint32_t channel;                                    // ADC_channels
    uint32_t samplingTime;                                // ADC_sampling_times
};

class AdcOneChannel : public mc_interfaces::AdcOneChannel {
public:
    AdcOneChannel (const AdcOneChannelCfg *const cfg,
                   uint32_t countCfg);
    
    mc_interfaces::res reinit (uint32_t numberCfg = 0);
    
    mc_interfaces::res startContinuousConversion (void);
    
    void stopContinuousConversion (void);
    
    uint32_t getMeasurement (void);
    
    void irq_handler (void);

private:
    void clkEnable (void);
    
    void clkDisable (void);
    
    const AdcOneChannelCfg *const cfg;
    const uint32_t countCfg;
    
    ADC_HandleTypeDef adc;
    ADC_ChannelConfTypeDef channelCfg;
};

}

#endif

#endif

