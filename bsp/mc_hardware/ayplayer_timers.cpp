#include "timer_counter.h"
#include "timer_interrupt.h"
#include "timer_pwm_one_channel.h"
#include "user_os.h"
#include "ayplayer_clock.h"
#include "FreeRTOSConfig.h"

/// TIM6 от APB1.
/// Генерирует прерывания 50 раз в секунду.
const McHardwareInterfacesImplementation::ClkTimBaseCfg timInterruptClkParam[ AYPLAYER_RCC_CFG_COUNT ] = {
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

const McHardwareInterfacesImplementation::TimInterruptCfg timInterruptCfg = {
	.tim			= TIM6,
	.cfg			= timInterruptClkParam,
	.countCfg		= AYPLAYER_RCC_CFG_COUNT,
};

McHardwareInterfacesImplementation::TimInterrupt interruptAy( &timInterruptCfg );

/// Подцветка дисплея.
/// 1 кГц, ШИМ.
const McHardwareInterfacesImplementation::ClkTimBaseCfg lcdPwmClkParam[ AYPLAYER_RCC_CFG_COUNT ] = {
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

const McHardwareInterfacesImplementation::TimPwmOneChannelCfg lcdPwmCfg = {
	.tim			= TIM1,
	.cfg			= lcdPwmClkParam,
	.countCfg		= AYPLAYER_RCC_CFG_COUNT,
	.outChannel		= TIM_CHANNEL_1,
	.polarity		= TIM_OCPOLARITY_LOW
};

McHardwareInterfacesImplementation::TimPwmOneChannel lcdPwm( &lcdPwmCfg );

#ifdef configGENERATE_RUN_TIME_STATS

/// Прерывания с частотый 20 кГц.
const McHardwareInterfacesImplementation::ClkTimBaseCfg tim5BaseCfg[ AYPLAYER_RCC_CFG_COUNT ] = {
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

McHardwareInterfacesImplementation::TimCounterCfg timRunTimeStatsCfg = {
	.tim					= TIM5,
	.cfg					= tim5BaseCfg,
	.countCfg				= AYPLAYER_RCC_CFG_COUNT
};

McHardwareInterfacesImplementation::TimCounter timRunTimeStats( &timRunTimeStatsCfg );

#endif
