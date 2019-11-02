/*

@startuml

namespace mc {

mc.Rcc ..|> mc.Rcc

class Rcc {
	{field}-	const RccCfg*				const cfg
	{field}-	const uint32_t				cfgCount
	{field}-	int						cfgNumberSet
	__Constructor__
	{method}+	Rcc	( const RccCfg*		const cfg,\n\t  uint32_t			cfgCount )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "platform.h"

#if defined( HAL_RCC_MODULE_ENABLED ) && defined ( HAL_FLASH_MODULE_ENABLED )

#define OSC_HSE_CFG                                                        \
        .OscillatorType                =    RCC_OSCILLATORTYPE_HSE,            \
        .HSEState                    =    RCC_HSE_ON,                        \
        .LSEState                    =    RCC_LSE_OFF,                    \
        .HSIState                    =    RCC_HSE_ON,                        \
        .HSICalibrationValue        =    RCC_HSICALIBRATION_DEFAULT,        \
        .LSIState                    =    RCC_LSI_OFF

#define PLL_ON(SOURCE, M, N, P, Q)                                            \
        .PLL = {                                                        \
            .PLLState                =    RCC_PLL_ON,                        \
            .PLLSource                =    SOURCE,                            \
            .PLLM                    =    M,                                \
            .PLLN                    =    N,                                \
            .PLLP                    =    P,                                \
            .PLLQ                    =    Q                                \
        }

#define PLL_OFF                                                            \
        .PLL = {                                                        \
            .PLLState                =    RCC_PLL_OFF,                    \
            .PLLSource                =    RCC_PLLSOURCE_HSI,                \
            .PLLM                    =    0,                                \
            .PLLN                    =    0,                                \
            .PLLP                    =    0,                                \
            .PLLQ                    =    0                                \
        }

#define CFG_ALL_BUS                                                        \
        .ClockType                    =    RCC_CLOCKTYPE_SYSCLK |            \
                                        RCC_CLOCKTYPE_HCLK |            \
                                        RCC_CLOCKTYPE_PCLK1 |            \
                                        RCC_CLOCKTYPE_PCLK2

#include "mc_clk.h"

namespace mc {

struct RccCfg {
    RCC_OscInitTypeDef osc;
    RCC_ClkInitTypeDef clk;
    uint32_t fLatency;
};

class Rcc : public mc_interfaces::clk {
public:
    Rcc (const RccCfg *const cfg,
         uint32_t cfgCount)
        : cfg(cfg), cfgCount(cfgCount) {}

public:
    mc_interfaces::RccResult setCfg (const uint32_t cfgNumberSet = 0);
    
    mc_interfaces::RccResult getCfgNumber (uint32_t &cfgNumber);

private:
    const RccCfg *const cfg;
    const uint32_t cfgCount;
    
    int cfgNumberSet = -1;
};

}

#endif

#endif
