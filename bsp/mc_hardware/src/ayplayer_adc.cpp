#include "ayplayer_mc_hardware.h"
#include "ayplayer_clock.h"

#define ADC_BAT_CHANNEL_CFG(PRESCALER) \
        { \
            .ADCx = ADC1, \
            .clockPrescaler = PRESCALER, \
            .resolution = ADC_RESOLUTION_12B, \
            .dataAlign = ADC_DATAALIGN_RIGHT, \
            .channel = ADC_CHANNEL_0, \
            .samplingTime = ADC_SAMPLETIME_480CYCLES \
        }

const mc::AdcOneChannelCfg adcBatCfg[] = {
    ADC_BAT_CHANNEL_CFG(ADC_CLOCK_SYNC_PCLK_DIV8),
    ADC_BAT_CHANNEL_CFG(ADC_CLOCK_SYNC_PCLK_DIV8),
    ADC_BAT_CHANNEL_CFG(ADC_CLOCK_SYNC_PCLK_DIV8),
    ADC_BAT_CHANNEL_CFG(ADC_CLOCK_SYNC_PCLK_DIV8)
};

mc::AdcOneChannel adcBat(adcBatCfg, AYPLAYER_RCC_CFG_COUNT);
