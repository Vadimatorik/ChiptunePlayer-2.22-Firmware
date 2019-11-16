#include "mc_hardware.h"

int init_mc_hardware () {
    int rv = 0;

    if ((rv = init_core()) != 0) {
        return rv;
    }

    if ((rv = init_gpio()) != 0) {
        return rv;
    }

    if ((rv = init_rcc()) != 0) {
        return rv;
    }

    if ((rv = init_uart()) != 0) {
        return rv;
    }

    if ((rv = init_spi_board()) != 0) {
        return rv;
    }

    if ((rv = init_spi_lcd()) != 0) {
        return rv;
    }

    if ((rv = init_tim_int_ay()) != 0) {
        return rv;
    }

    if ((rv = init_tim_lcd_pwm()) != 0) {
        return rv;
    }

    if ((rv = set_tim_lcd_pwm_duty(0.5)) != 0) {
        return rv;
    }

    if ((rv = start_tim_lcd_pwm()) != 0) {
        return rv;
    }

    return rv;
}