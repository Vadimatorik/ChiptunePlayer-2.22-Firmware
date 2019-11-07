#include "mc_hardware.h"

#include "stm32f4xx_hal_spi.h"

void HAL_SPI_TxCpltCallback (SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI1) {
        set_pin_lcd_cs();
    } else {
        set_pin_ad5204_cs();
    }
}
