#include "gpio.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"

int init_gpio() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOC, BOARD_GPIO_1_Pin | BOARD_GPIO_2_Pin | BOARD_GPIO_3_Pin | BR0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PWR_5V_ON_Pin | BOARD_GPIO_4_Pin | USB_ID_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, LCD_A0_Pin | LCD_CS_Pin | LCD_RES_Pin | SD_CD1_Pin
                             | BR2_Pin | BR1_Pin | PWR_ON_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = BAT_STDBY_Pin | BC1_Pin | BC2_Pin | UP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BOARD_GPIO_1_Pin | BOARD_GPIO_2_Pin | BOARD_GPIO_3_Pin | BR0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PWR_5V_ON_Pin | BOARD_GPIO_4_Pin | USB_ID_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BC0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BC0_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_A0_Pin | LCD_CS_Pin | LCD_RES_Pin | SD_CD1_Pin
                          | BR2_Pin | BR1_Pin | PWR_ON_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SD_PUSH_Pin | NOT_USED_Pin | BAT_CHRG_Pin | DOWN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
