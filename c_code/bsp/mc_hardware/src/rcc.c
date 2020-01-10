#ifdef HARD
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_hal_flash.h"
#endif

#include <errno.h>

int init_rcc () {
#ifdef HARD
    RCC_OscInitTypeDef osc_cfg = {0};
    RCC_ClkInitTypeDef clk_cfg = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    osc_cfg.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc_cfg.HSEState = RCC_HSE_ON;
    osc_cfg.PLL.PLLState = RCC_PLL_ON;
    osc_cfg.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    osc_cfg.PLL.PLLM = 16;
    osc_cfg.PLL.PLLN = 192;
    osc_cfg.PLL.PLLP = RCC_PLLP_DIV2;
    osc_cfg.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&osc_cfg) != HAL_OK) {
        return EIO;
    }

    clk_cfg.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk_cfg.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
    clk_cfg.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk_cfg.APB1CLKDivider = RCC_HCLK_DIV1;
    clk_cfg.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&clk_cfg, FLASH_LATENCY_0) != HAL_OK) {
        return EIO;
    }
#endif

    return 0;
}