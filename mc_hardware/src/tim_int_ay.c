#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"

#include <errno.h>

TIM_HandleTypeDef tim_int_ay = {0};

int init_tim_int_ay () {
    __HAL_RCC_TIM6_CLK_ENABLE();

    TIM_MasterConfigTypeDef sMasterConfig = {0};

    tim_int_ay.Instance = TIM6;
    tim_int_ay.Init.Prescaler = 31;
    tim_int_ay.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim_int_ay.Init.Period = 3199;
    tim_int_ay.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&tim_int_ay) != HAL_OK) {
        return EIO;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&tim_int_ay, &sMasterConfig) != HAL_OK) {
        return EIO;
    }

    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

    return 0;
}

int start_tim_int_ay () {
    return (HAL_TIM_Base_Start_IT(&tim_int_ay) == HAL_OK)?:EIO;
}