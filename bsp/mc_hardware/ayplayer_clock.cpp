#include "ayplayer_clock.h"

const RccCfg clockCfg[ AYPLAYER_RCC_CFG_COUNT ] = {
	{
		.osc = {
			OSC_HSE_CFG,
			PLL_ON( RCC_PLLSOURCE_HSE, 8, 72, RCC_PLLP_DIV4, 3 )
		},
		.clk = {
			CFG_ALL_BUS,
			.SYSCLKSource				=	RCC_SYSCLKSOURCE_PLLCLK,
			.AHBCLKDivider				=	RCC_SYSCLK_DIV2,
			.APB1CLKDivider				=	RCC_HCLK_DIV4,
			.APB2CLKDivider				=	RCC_HCLK_DIV4,
		},
		.fLatency						=	FLASH_LATENCY_0
	},
	{
		.osc = {
			OSC_HSE_CFG,
			PLL_ON( RCC_PLLSOURCE_HSE, 8, 72, RCC_PLLP_DIV4, 3 )
		},
		.clk = {
			CFG_ALL_BUS,
			.SYSCLKSource				=	RCC_SYSCLKSOURCE_PLLCLK,
			.AHBCLKDivider				=	RCC_SYSCLK_DIV1,
			.APB1CLKDivider				=	RCC_HCLK_DIV2,
			.APB2CLKDivider				=	RCC_HCLK_DIV2,
		},
		.fLatency						=	FLASH_LATENCY_1
	},
	{
		.osc = {
			OSC_HSE_CFG,
			PLL_ON( RCC_PLLSOURCE_HSE, 8, 72, RCC_PLLP_DIV2, 3 )
		},
		.clk = {
			CFG_ALL_BUS,
			.SYSCLKSource				=	RCC_SYSCLKSOURCE_PLLCLK,
			.AHBCLKDivider				=	RCC_SYSCLK_DIV1,
			.APB1CLKDivider				=	RCC_HCLK_DIV2,
			.APB2CLKDivider				=	RCC_HCLK_DIV2,
		},
		.fLatency						=	FLASH_LATENCY_2
	},
	{
		.osc = {
			OSC_HSE_CFG,
			PLL_ON( RCC_PLLSOURCE_HSE, 8, 168, RCC_PLLP_DIV2, 7 )
		},
		.clk = {
			CFG_ALL_BUS,
			.SYSCLKSource				=	RCC_SYSCLKSOURCE_PLLCLK,
			.AHBCLKDivider				=	RCC_SYSCLK_DIV1,
			.APB1CLKDivider				=	RCC_HCLK_DIV4,
			.APB2CLKDivider				=	RCC_HCLK_DIV2,
		},
		.fLatency						=	FLASH_LATENCY_5
	},
};

Rcc rcc( clockCfg, AYPLAYER_RCC_CFG_COUNT );
