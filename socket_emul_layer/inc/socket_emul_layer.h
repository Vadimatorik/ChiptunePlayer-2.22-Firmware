#pragma once

#include <stdint.h>
#include "socket_emul_layer_struct.h"

#define SOCKET_PORT_PIN_LCD_RST 50000
#define SOCKET_PORT_PIN_LCD_DC 50001
#define SOCKET_PORT_PIN_LCD_CS 50002

#define SOCKET_SPI_LCD 50100

int get_socket_fd (uint16_t port);

void socket_gpio_set (int fd);
void socket_gpio_reset (int fd);

void socket_spi_tx (int fd, void *d, uint32_t len);
