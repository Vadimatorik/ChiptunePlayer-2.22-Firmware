#include "tim_cnt.h"
#include "tim_int.h"
#include "tim_pwm_one_ch.h"
#include "user_os.h"
#include "ayplayer_clock.h"
#include "FreeRTOSConfig.h"

/// TIM6 от APB1.
/// Генерирует прерывания 50 раз в секунду.
const mc::ClkTimBaseCfg timInterruptClkParam[ AYPLAYER_RCC_CFG_COUNT ] = {
	{
		.period				=	500 - 1,
		.prescaler			=	360 - 1,
		.pulse				=	0
	}, {
		.period				=	200 - 1,
		.prescaler			=	3600 - 1,
		.pulse				=	0
	}, {
		.period				=	400 - 1,
		.prescaler			=	3600 - 1,
		.pulse				=	0
	}, {
		.period				=	400 - 1,
		.prescaler			=	4200 - 1,
		.pulse				=	0
	},
};

const mc::TimInterruptCfg timInterruptCfg = {
	.tim			= TIM6,
	.cfg			= timInterruptClkParam,
	.countCfg		= AYPLAYER_RCC_CFG_COUNT,
};

mc::TimInterrupt interruptAy( &timInterruptCfg );

/// Подцветка дисплея.
/// 1 кГц, ШИМ.
const mc::ClkTimBaseCfg lcdPwmClkParam[ AYPLAYER_RCC_CFG_COUNT ] = {
	{
		.period				= 	9 - 1,
		.prescaler			=	500 - 1,
		.pulse				=	0,
	}, {
		.period				=	18 - 1,
		.prescaler			=	1000 - 1,
		.pulse				=	0
	}, {
		.period				=	36 - 1,
		.prescaler			=	1000 - 1,
		.pulse				=	0
	}, {
		.period				=	168 - 1,
		.prescaler			=	1000 - 1,
		.pulse				=	0
	},
};

const mc::TimPwmOneChannelCfg lcdPwmCfg = {
	.tim			= TIM1,
	.cfg			= lcdPwmClkParam,
	.countCfg		= AYPLAYER_RCC_CFG_COUNT,
	.outChannel		= TIM_CHANNEL_1,
	.polarity		= TIM_OCPOLARITY_LOW
};

mc::TimPwmOneChannel lcdPwm( &lcdPwmCfg );

#ifdef configGENERATE_RUN_TIME_STATS

/// Прерывания с частотый 20 кГц.
const mc::ClkTimBaseCfg tim5BaseCfg[ AYPLAYER_RCC_CFG_COUNT ] = {
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

mc::TimCounterCfg timRunTimeStatsCfg = {
	.tim					= TIM5,
	.cfg					= tim5BaseCfg,
	.countCfg				= AYPLAYER_RCC_CFG_COUNT
};

mc::TimCounter timRunTimeStats( &timRunTimeStatsCfg );

#endif
