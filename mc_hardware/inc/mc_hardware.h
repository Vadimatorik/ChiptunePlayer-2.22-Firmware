#pragma once

#include <stdint.h>

typedef enum {
    SPI_BOARD_DEVICE_NO_SET = 0,
    SPI_BOARD_DEVICE_AD5204 = 1,
    SPI_BOARD_DEVICE_SR = 2,
    SPI_BOARD_DEVICE_LTC6903 = 3
} SPI_BOARD_DEVICE;

int init_core ();
int init_gpio ();
int init_rcc ();
int init_uart ();
int init_spi_board ();
int init_spi_lcd ();
int init_tim_lcd_pwm ();
int init_tim_int_ay ();

int start_tim_lcd_pwm ();
int start_tim_int_ay ();

int set_tim_lcd_pwm_duty (float duty);

int spi_lcd_tx (void *d, uint32_t len);
int spi_board_device_ltc6903_tx (void *d, uint32_t len);
int spi_board_device_ad5204_tx (void *d, uint32_t len);
int spi_board_device_sr_tx (void *d, uint32_t len);

void set_pin_pwr_5_v ();
void set_pin_pwr_3_v_3 ();
void set_pin_ltc_cs ();
void set_pin_ad5204_cs ();
void set_pin_sr_strob ();
void set_pin_lcd_cs ();
void set_pin_lcd_dc ();
void set_pin_lcd_rst ();

void reset_pin_pwr_5_v ();
void reset_pin_pwr_3_v_3 ();
void reset_pin_ltc_cs ();
void reset_pin_ad5204_cs ();
void reset_pin_sr_strob ();
void reset_pin_lcd_cs ();
void reset_pin_lcd_dc ();
void reset_pin_lcd_rst ();