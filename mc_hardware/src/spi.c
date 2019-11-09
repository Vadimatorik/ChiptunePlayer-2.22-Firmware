#include "mc_hardware.h"

#include "stm32f4xx_hal_spi.h"

extern SPI_BOARD_DEVICE spi_device;

void HAL_SPI_TxCpltCallback (SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI1) {
        set_pin_lcd_cs();
    } else {
        switch (spi_device) {
            case SPI_BOARD_DEVICE_AD5204:
                set_pin_ad5204_cs();
                break;

            case SPI_BOARD_DEVICE_SR:
                set_pin_sr_strob();
                reset_pin_sr_strob();
                break;

            case SPI_BOARD_DEVICE_LTC6903:
                set_pin_ltc_cs();
                break;
        }
    }
}
