#ifdef HARD
#include "gpio.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#endif

#ifdef SOFT
#include "socket_emul_layer.h"
#endif

int init_gpio () {
#if defined(HARD)
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, BOARD_GPIO_1|BOARD_GPIO_2|BOARD_GPIO_3|BR0, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, PWR_5V_ON|BOARD_GPIO_4|USB_ID, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, LCD_A0|LCD_CS|LCD_RES|SD_CD1
                             |BR2|BR1|PWR_ON, GPIO_PIN_RESET);

    /*Configure GPIO pins : BAT_STDBY UP */
    GPIO_InitStruct.Pin = BAT_STDBY|UP;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : BOARD_GPIO_1 BOARD_GPIO_2 BOARD_GPIO_3 */
    GPIO_InitStruct.Pin = BOARD_GPIO_1|BOARD_GPIO_2|BOARD_GPIO_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PWR_5V_ON BOARD_GPIO_4 USB_ID */
    GPIO_InitStruct.Pin = PWR_5V_ON|BOARD_GPIO_4|USB_ID;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : BC0 */
    GPIO_InitStruct.Pin = BC0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BC0_GPIO, &GPIO_InitStruct);

    /*Configure GPIO pins : BC1 BC2 */
    GPIO_InitStruct.Pin = BC1|BC2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : LCD_A0 LCD_CS LCD_RES SD_CD1 
                             PWR_ON */
    GPIO_InitStruct.Pin = LCD_A0|LCD_CS|LCD_RES|SD_CD1
                          |PWR_ON;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : SD_PUSH NOT_USED BAT_CHRG DOWN */
    GPIO_InitStruct.Pin = SD_PUSH|NOT_USED|BAT_CHRG|DOWN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : BR2 BR1 */
    GPIO_InitStruct.Pin = BR2|BR1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : BR0 */
    GPIO_InitStruct.Pin = BR0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(BR0_GPIO, &GPIO_InitStruct);

    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
    GPIO_InitStruct.Pin = ADC_BAT;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ADC_BAT_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AMPLIFIER_LEFT;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AMPLIFIER_LEFT_GPIO, &GPIO_InitStruct);

    /**SDIO GPIO Configuration    
   PC8     ------> SDIO_D0
   PC9     ------> SDIO_D1
   PC10     ------> SDIO_D2
   PC11     ------> SDIO_D3
   PC12     ------> SDIO_CK
   PD2     ------> SDIO_CMD 
   */
    GPIO_InitStruct.Pin = SD_D0|SD_D1|SD_D2|SD_D3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SD_CLK;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(SD_CLK_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SD_CMD;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(SD_CMD_GPIO, &GPIO_InitStruct);

    /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA7     ------> SPI1_MOSI
        */
    GPIO_InitStruct.Pin = LCD_CLK|LCD_TX;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /**SPI2 GPIO Configuration
   PC3     ------> SPI2_MOSI
   PB10     ------> SPI2_SCK
   */
    GPIO_InitStruct.Pin = BOARD_TX;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(BOARD_TX_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BOARD_CLK;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(BOARD_CLK_GPIO, &GPIO_InitStruct);

    /**TIM1 GPIO Configuration
PA8     ------> TIM1_CH1
*/
    GPIO_InitStruct.Pin = LCD_PWM;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(LCD_PWM_GPIO, &GPIO_InitStruct);

    /**USART1 GPIO Configuration
PB6     ------> USART1_TX
PB7     ------> USART1_RX
*/
    GPIO_InitStruct.Pin = DEBUG_TX|DEBUG_RX;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

#elif defined(SOFT)
#endif

    return 0;
}

#ifdef HARD
#define LTC_CS_GPIO BOARD_GPIO_4_GPIO
#define AD5204_CS_GPIO BOARD_GPIO_3_GPIO
#define SH_STROB_GPIO BOARD_GPIO_2_GPIO

#define LTC_CS BOARD_GPIO_4
#define AD5204_CS BOARD_GPIO_3
#define SH_STROB BOARD_GPIO_2
#endif

void set_pin_pwr_5_v () {
#ifdef HARD
    HAL_GPIO_WritePin(PWR_5V_ON_GPIO, PWR_5V_ON, GPIO_PIN_SET);
#endif
}

void set_pin_pwr_3_v_3 () {
#ifdef HARD
    HAL_GPIO_WritePin(PWR_ON_GPIO, PWR_ON, GPIO_PIN_SET);
#endif
}

void set_pin_ltc_cs () {
#ifdef HARD
    HAL_GPIO_WritePin(LTC_CS_GPIO, LTC_CS, GPIO_PIN_SET);
#endif
}

void set_pin_ad5204_cs () {
#ifdef HARD
    HAL_GPIO_WritePin(AD5204_CS_GPIO, AD5204_CS, GPIO_PIN_SET);
#endif
}

void set_pin_sr_strob () {
#ifdef HARD
    HAL_GPIO_WritePin(SH_STROB_GPIO, SH_STROB, GPIO_PIN_SET);
#endif
}

void set_pin_lcd_cs () {
#if defined(HARD)
    HAL_GPIO_WritePin(LCD_CS_GPIO, LCD_CS, GPIO_PIN_SET);
#elif defined(SOFT)
    socket_gpio_lcd_cs_set(0xFF);
#endif
}

void set_pin_lcd_dc () {
#if defined(HARD)
    HAL_GPIO_WritePin(LCD_A0_GPIO, LCD_A0, GPIO_PIN_SET);
#elif defined(SOFT)
    socket_gpio_lcd_dc_set(0xFF);
#endif
}

void set_pin_lcd_rst () {
#if defined(HARD)
    HAL_GPIO_WritePin(LCD_RES_GPIO, LCD_RES, GPIO_PIN_SET);
#elif defined(SOFT)
    socket_gpio_lcd_rst_set(0xFF);
#endif
}

void set_pin_br_0 () {
#if defined(HARD)
    HAL_GPIO_WritePin(BR0_GPIO, BR0, GPIO_PIN_SET);
#endif
}

void set_pin_br_1 () {
#if defined(HARD)
    HAL_GPIO_WritePin(BR1_GPIO, BR1, GPIO_PIN_SET);
#endif
}

void set_pin_br_2 () {
#if defined(HARD)
    HAL_GPIO_WritePin(BR2_GPIO, BR2, GPIO_PIN_SET);
#endif
}

void reset_pin_pwr_5_v () {
#ifdef HARD
    HAL_GPIO_WritePin(PWR_5V_ON_GPIO, PWR_5V_ON, GPIO_PIN_RESET);
#endif
}

void reset_pin_pwr_3_v_3 () {
#ifdef HARD
    HAL_GPIO_WritePin(PWR_ON_GPIO, PWR_ON, GPIO_PIN_RESET);
#endif
}

void reset_pin_ltc_cs () {
#ifdef HARD
    HAL_GPIO_WritePin(LTC_CS_GPIO, LTC_CS, GPIO_PIN_RESET);
#endif
}

void reset_pin_ad5204_cs () {
#ifdef HARD
    HAL_GPIO_WritePin(AD5204_CS_GPIO, AD5204_CS, GPIO_PIN_RESET);
#endif
}

void reset_pin_sr_strob () {
#ifdef HARD
    HAL_GPIO_WritePin(SH_STROB_GPIO, SH_STROB, GPIO_PIN_RESET);
#endif
}

void reset_pin_lcd_cs () {
#if defined(HARD)
    HAL_GPIO_WritePin(LCD_CS_GPIO, LCD_CS, GPIO_PIN_RESET);
#elif defined(SOFT)
    socket_gpio_lcd_cs_set(0);
#endif
}

void reset_pin_lcd_dc () {
#if defined(HARD)
    HAL_GPIO_WritePin(LCD_A0_GPIO, LCD_A0, GPIO_PIN_RESET);
#elif defined(SOFT)
    socket_gpio_lcd_dc_set(0);
#endif
}

void reset_pin_lcd_rst () {
#if defined(HARD)
    HAL_GPIO_WritePin(LCD_RES_GPIO, LCD_RES, GPIO_PIN_RESET);
#elif defined(SOFT)
    socket_gpio_lcd_rst_set(0);
#endif
}

void reset_pin_br_0 () {
#if defined(HARD)
    HAL_GPIO_WritePin(BR0_GPIO, BR0, GPIO_PIN_RESET);
#endif
}

void reset_pin_br_1 () {
#if defined(HARD)
    HAL_GPIO_WritePin(BR1_GPIO, BR1, GPIO_PIN_RESET);
#endif
}

void reset_pin_br_2 () {
#if defined(HARD)
    HAL_GPIO_WritePin(BR2_GPIO, BR2, GPIO_PIN_RESET);
#endif
}

uint8_t read_pin_bc_0 () {
#if defined(HARD)
    return HAL_GPIO_ReadPin(BC0_GPIO, BC0);
#endif
}

uint8_t read_pin_bc_1 () {
#if defined(HARD)
    return HAL_GPIO_ReadPin(BC1_GPIO, BC1);
#endif
}

uint8_t read_pin_bc_2 () {
#if defined(HARD)
    return HAL_GPIO_ReadPin(BC2_GPIO, BC2);
#endif
}