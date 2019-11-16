#ifdef AYM_HARDWARE
#include "gpio.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#endif

int init_gpio () {
#ifdef AYM_HARDWARE
    GPIO_InitTypeDef cfg = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    // Обычные выводы.
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
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &cfg);

    cfg.Pin = PWR_5V_ON | BOARD_GPIO_4 | USB_ID;
    cfg.Mode = GPIO_MODE_OUTPUT_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &cfg);

    cfg.Pin = BC0;
    cfg.Mode = GPIO_MODE_INPUT;
    cfg.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BC0_GPIO, &cfg);

    cfg.Pin = LCD_A0 | LCD_CS | LCD_RES | SD_CD1
              | BR2 | BR1 | PWR_ON;
    cfg.Mode = GPIO_MODE_OUTPUT_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin = SD_PUSH | NOT_USED | BAT_CHRG | DOWN;
    cfg.Mode = GPIO_MODE_INPUT;
    cfg.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &cfg);

    // UART.
    cfg.Pin = DEBUG_TX | DEBUG_RX;
    cfg.Mode = GPIO_MODE_AF_PP;
    cfg.Pull = GPIO_PULLUP;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    cfg.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &cfg);

    // SPI_LCD.
    cfg.Pin = LCD_CLK | LCD_TX;
    cfg.Mode = GPIO_MODE_AF_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    cfg.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &cfg);

    // SPI_BOARD.
    cfg.Pin = BOARD_TX;
    cfg.Mode = GPIO_MODE_AF_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    cfg.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(BOARD_TX_GPIO, &cfg);

    cfg.Pin = BOARD_CLK;
    cfg.Mode = GPIO_MODE_AF_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    cfg.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(BOARD_CLK_GPIO, &cfg);

    // TIM_LCD_PWM.
    cfg.Pin = LCD_PWM;
    cfg.Mode = GPIO_MODE_AF_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    cfg.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(LCD_PWM_GPIO, &cfg);

    // SDIO.
    cfg.Pin = SD_D0 | SD_D1 | SD_D2 | SD_D3
              | SD_CLK;
    cfg.Mode = GPIO_MODE_AF_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    cfg.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &cfg);

    cfg.Pin = SD_CMD;
    cfg.Mode = GPIO_MODE_AF_PP;
    cfg.Pull = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    cfg.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(SD_CMD_GPIO_Port, &cfg);
#endif

    return 0;
}

#ifdef AYM_HARDWARE
#define LTC_CS_GPIO BOARD_GPIO_4_GPIO_Port
#define AD5204_CS_GPIO BOARD_GPIO_3_GPIO_Port
#define SH_STROB_GPIO BOARD_GPIO_2_GPIO_Port

#define LTC_CS BOARD_GPIO_4
#define AD5204_CS BOARD_GPIO_3
#define SH_STROB BOARD_GPIO_2
#endif

void set_pin_pwr_5_v () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(PWR_5V_ON_GPIO, PWR_5V_ON, GPIO_PIN_SET);
#endif
}

void set_pin_pwr_3_v_3 () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(PWR_ON_GPIO, PWR_ON, GPIO_PIN_SET);
#endif
}

void set_pin_ltc_cs () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(LTC_CS_GPIO, LTC_CS, GPIO_PIN_SET);
#endif
}

void set_pin_ad5204_cs () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(AD5204_CS_GPIO, AD5204_CS, GPIO_PIN_SET);
#endif
}

void set_pin_sr_strob () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(SH_STROB_GPIO, SH_STROB, GPIO_PIN_SET);
#endif
}

void set_pin_lcd_cs () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(LCD_CS_GPIO, LCD_CS, GPIO_PIN_SET);
#endif
}

void set_pin_lcd_dc () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(LCD_A0_GPIO, LCD_A0, GPIO_PIN_SET);
#endif
}

void set_pin_lcd_rst () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(LCD_RES_GPIO, LCD_RES, GPIO_PIN_SET);
#endif
}

void reset_pin_pwr_5_v () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(PWR_5V_ON_GPIO, PWR_5V_ON, GPIO_PIN_RESET);
#endif
}

void reset_pin_pwr_3_v_3 () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(PWR_ON_GPIO, PWR_ON, GPIO_PIN_RESET);
#endif
}

void reset_pin_ltc_cs () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(LTC_CS_GPIO, LTC_CS, GPIO_PIN_RESET);
#endif
}

void reset_pin_ad5204_cs () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(AD5204_CS_GPIO, AD5204_CS, GPIO_PIN_RESET);
#endif
}

void reset_pin_sr_strob () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(SH_STROB_GPIO, SH_STROB, GPIO_PIN_RESET);
#endif
}

void reset_pin_lcd_cs () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(LCD_CS_GPIO, LCD_CS, GPIO_PIN_RESET);
#endif
}

void reset_pin_lcd_dc () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(LCD_A0_GPIO, LCD_A0, GPIO_PIN_RESET);
#endif
}

void reset_pin_lcd_rst () {
#ifdef AYM_HARDWARE
    HAL_GPIO_WritePin(LCD_RES_GPIO, LCD_RES, GPIO_PIN_RESET);
#endif
}

