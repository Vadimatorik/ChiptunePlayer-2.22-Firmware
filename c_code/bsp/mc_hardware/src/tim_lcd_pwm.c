#ifdef AYM_HARDWARE
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"
#endif

#include <errno.h>

#ifdef AYM_HARDWARE
__attribute__ ((section (".bss_ccm")))
TIM_HandleTypeDef tim_lcd_pwm = {0};
#endif

int init_tim_lcd_pwm () {
#ifdef AYM_HARDWARE
    __HAL_RCC_TIM1_CLK_ENABLE();

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    tim_lcd_pwm.Instance = TIM1;
    tim_lcd_pwm.Init.Prescaler = 16 - 1;
    tim_lcd_pwm.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim_lcd_pwm.Init.Period = 1000 - 1;
    tim_lcd_pwm.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim_lcd_pwm.Init.RepetitionCounter = 0;
    tim_lcd_pwm.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&tim_lcd_pwm) != HAL_OK) {
        return EIO;
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&tim_lcd_pwm, &sClockSourceConfig) != HAL_OK) {
        return EIO;
    }

    if (HAL_TIM_PWM_Init(&tim_lcd_pwm) != HAL_OK) {
        return EIO;
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&tim_lcd_pwm, &sMasterConfig) != HAL_OK) {
        return EIO;
    }

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 999;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&tim_lcd_pwm, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        return EIO;
    }

    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&tim_lcd_pwm, &sBreakDeadTimeConfig) != HAL_OK) {
        return EIO;
    }
#endif

    return 0;
}

int set_tim_lcd_pwm_duty (float duty) {
    if ((duty > 1) || (duty < 0)) {
        return EINVAL;
    }

#ifdef AYM_HARDWARE
    __HAL_TIM_SET_COMPARE(&tim_lcd_pwm, TIM_CHANNEL_1, (1000 - 1)*duty);
#endif

    return 0;
}

int start_tim_lcd_pwm () {
#ifdef AYM_HARDWARE
    return (HAL_TIM_PWM_Start(&tim_lcd_pwm, TIM_CHANNEL_1) == HAL_OK)?0:EIO;
#else
    return 0;
#endif
}