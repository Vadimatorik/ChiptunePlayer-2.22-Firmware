#include "timer.h"
#include "user_os.h"
#include "ayplayer_clock.h"

/// TIM6 от APB1.
/// Генерирует прерывания 50 раз в секунду.
const clkTimBaseCfg timInterruptClkParam[ AYPLAYER_RCC_CFG_COUNT ] = {
	{
		.period				=	9000 - 1,
		.prescaler			=	10 - 1,
		.pulse				=	0
	}, {
		.period				=	36000 - 1,
		.prescaler			=	10 - 1,
		.pulse				=	0
	}, {
		.period				=	7200 - 1,
		.prescaler			=	100,
		.pulse				=	0
	}, {
		.period				=	100,
		.prescaler			=	8400 - 1,
		.pulse				=	0
	},
};

const timInterruptCfg timInterruptCfg = {
	.tim			= TIM6,
	.cfg			= timInterruptClkParam,
	.countCfg		= AYPLAYER_RCC_CFG_COUNT,
};

TimInterrupt interruptAy( &timInterruptCfg );

/// Подцветка дисплея.
/// 1 кГц, ШИМ.
const clkTimBaseCfg lcdPwmClkParam[ AYPLAYER_RCC_CFG_COUNT ] = {
	{
		.period				= 	500 - 1,
		.prescaler			=	1000 - 1,
		.pulse				=	0,
	}, {
		.period				=	18000 - 1,
		.prescaler			=	1000 - 1,
		.pulse				=	0
	}, {
		.period				=	36000 - 1,
		.prescaler			=	1000 - 1,
		.pulse				=	0
	}, {
		.period				=	8400,
		.prescaler			=	10000 - 1,
		.pulse				=	0
	},
};

const timPwmOneChannelCfg lcdPwmCfg = {
	.tim			= TIM1,
	.cfg			= lcdPwmClkParam,
	.countCfg		= AYPLAYER_RCC_CFG_COUNT,
	.outChannel		= TIM_CHANNEL_1,
	.polarity		= TIM_OCPOLARITY_LOW
};

TimPwmOneChannel lcdPwm( &lcdPwmCfg );

#ifdef configGENERATE_RUN_TIME_STATS

/// Прерывания с частотый 20 кГц.
const clkTimBaseCfg tim5BaseCfg[ AYPLAYER_RCC_CFG_COUNT ] = {
	{
		.period				=	0xFFFFFFFF,
		.prescaler			=	225 - 1,
		.pulse				=	0
	}, {
		.period				=	0xFFFFFFFF,
		.prescaler			=	900 - 1,
		.pulse				=	0
	}, {
		.period				=	0xFFFFFFFF,
		.prescaler			=	1800 - 1,
		.pulse				=	0
	}, {
		.period				=	0xFFFFFFFF,
		.prescaler			=	2100 - 1,
		.pulse				=	0
	},
};

timCounterCfg timRunTimeStatsCfg = {
	.tim					= TIM5,
	.cfg					= tim5BaseCfg,
	.countCfg				= AYPLAYER_RCC_CFG_COUNT
};

TimCounter timRunTimeStats( &timRunTimeStatsCfg );

#endif
