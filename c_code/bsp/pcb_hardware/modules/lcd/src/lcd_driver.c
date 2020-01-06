#include "u8g2.h"
#include "mc_hardware.h"
#include "freertos_headers.h"

uint8_t u8x8_byte_send (U8X8_UNUSED u8x8_t *u8x8, uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_BYTE_INIT:
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
        case U8X8_MSG_BYTE_END_TRANSFER:
            break;

        case U8X8_MSG_BYTE_SEND:
            spi_lcd_tx(arg_ptr, arg_int);
            return arg_int;

        case U8X8_MSG_BYTE_SET_DC:
            u8x8_gpio_SetDC(u8x8, arg_int);
            break;

        default:
            while (1);
    }

    return 1;
}

uint8_t u8x8_io (U8X8_UNUSED u8x8_t *u8x8, uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            break;

        case U8X8_MSG_GPIO_DC:
            if (arg_int) {
                set_pin_lcd_dc();
            } else {
                reset_pin_lcd_dc();
            }
            break;

        case U8X8_MSG_GPIO_RESET:
            if (arg_int) {
                set_pin_lcd_rst();
            } else {
                reset_pin_lcd_rst();
            }
            break;

        case U8X8_MSG_DELAY_MILLI:
            vTaskDelay(arg_int);
            break;

        default:
            while (1);
    }
    return 1;
}