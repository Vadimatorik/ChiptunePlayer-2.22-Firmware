#pragma once

#include <stdint.h>

int init_core ();
int init_gpio ();
int init_rcc ();
int init_uart ();
int init_spi_board ();
int init_spi_lcd ();

void get_string_from_uart (char* buf, uint32_t max_len);

int spi_board_device_ad5204_tx (void *d, uint32_t len);

void set_pin_ad5204_cs ();
void reset_pin_ad5204_cs ();

void set_pin_lcd_cs ();
void reset_pin_lcd_cs ();