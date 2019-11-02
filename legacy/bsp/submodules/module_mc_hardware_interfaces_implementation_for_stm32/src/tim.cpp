#include "platform.h"

#ifdef HAL_TIM_MODULE_ENABLED

namespace mc {

// Включаем тактирование SPI.
void clkTimInit (TIM_TypeDef *tim) {
    switch ((uint32_t)tim) {
#ifdef TIM
        case	TIM_BASE:		__HAL_RCC_TIM_CLK_ENABLE();			break;
#endif
#ifdef TIM1
        case TIM1_BASE:
            __HAL_RCC_TIM1_CLK_ENABLE();
            break;
#endif
#ifdef TIM2
        case TIM2_BASE:
            __HAL_RCC_TIM2_CLK_ENABLE();
            break;
#endif
#ifdef TIM3
        case TIM3_BASE:
            __HAL_RCC_TIM3_CLK_ENABLE();
            break;
#endif
#ifdef TIM4
        case TIM4_BASE:
            __HAL_RCC_TIM4_CLK_ENABLE();
            break;
#endif
#ifdef TIM5
        case TIM5_BASE:
            __HAL_RCC_TIM5_CLK_ENABLE();
            break;
#endif
#ifdef TIM6
        case TIM6_BASE:
            __HAL_RCC_TIM6_CLK_ENABLE();
            break;
#endif
#ifdef TIM7
        case TIM7_BASE:
            __HAL_RCC_TIM7_CLK_ENABLE();
            break;
#endif
#ifdef TIM8
        case TIM8_BASE:
            __HAL_RCC_TIM8_CLK_ENABLE();
            break;
#endif
#ifdef TIM9
        case TIM9_BASE:
            __HAL_RCC_TIM9_CLK_ENABLE();
            break;
#endif
#ifdef TIM10
        case TIM10_BASE:
            __HAL_RCC_TIM10_CLK_ENABLE();
            break;
#endif
#ifdef TIM11
        case TIM11_BASE:
            __HAL_RCC_TIM11_CLK_ENABLE();
            break;
#endif
#ifdef TIM12
        case TIM12_BASE:
            __HAL_RCC_TIM12_CLK_ENABLE();
            break;
#endif
#ifdef TIM13
        case TIM13_BASE:
            __HAL_RCC_TIM13_CLK_ENABLE();
            break;
#endif
#ifdef TIM14
        case TIM14_BASE:
            __HAL_RCC_TIM14_CLK_ENABLE();
            break;
#endif
#ifdef TIM15
            case	TIM15_BASE:		__HAL_RCC_TIM15_CLK_ENABLE();		break;
#endif
#ifdef TIM16
            case	TIM16_BASE:		__HAL_RCC_TIM16_CLK_ENABLE();		break;
#endif
    };
}

}

#endif
