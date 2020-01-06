#pragma once

#include <stdint.h>
#include "socket_emul_layer_struct.h"

int init_sockets ();

void socket_gpio_lcd_cs_set  (uint8_t state);
void socket_gpio_lcd_dc_set (uint8_t state);
void socket_gpio_lcd_rst_set (uint8_t state);

void socket_spi_lcd_tx (void *d, uint32_t len);

uint8_t socket_get_button_state (uint8_t i);
