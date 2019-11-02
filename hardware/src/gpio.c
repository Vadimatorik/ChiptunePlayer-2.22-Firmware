#include "gpio.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"

int init_gpio () {
    GPIO_InitTypeDef cfg = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOC, BOARD_GPIO_1 | BOARD_GPIO_2 | BOARD_GPIO_3 | BR0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PWR_5V_ON | BOARD_GPIO_4 | USB_ID, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, LCD_A0 | LCD_CS | LCD_RES | SD_CD1
                             | BR2 | BR1 | PWR_ON, GPIO_PIN_RESET);

    cfg.Pin = BAT_STDBY | BC1 | BC2 | UP;
    cfg.Mode = GPIO_MODE_INPUT;
    cfg.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &cfg);

    cfg.Pin = BOARD_GPIO_1 | BOARD_GPIO_2 | BOARD_GPIO_3 | BR0;
    cfg.Mode = GPIO_MODE_OUTPUT_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &cfg);

    cfg.Pin = PWR_5V_ON | BOARD_GPIO_4 | USB_ID;
    cfg.Mode = GPIO_MODE_OUTPUT_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &cfg);

    cfg.Pin = BC0;
    cfg.Mode = GPIO_MODE_INPUT;
    cfg.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BC0_GPIO_Port, &cfg);

    cfg.Pin = LCD_A0 | LCD_CS | LCD_RES | SD_CD1
              | BR2 | BR1 | PWR_ON;
    cfg.Mode = GPIO_MODE_OUTPUT_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin = SD_PUSH | NOT_USED | BAT_CHRG | DOWN;
    cfg.Mode = GPIO_MODE_INPUT;
    cfg.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &cfg);

    return 0;
}
