#include "freertos_headers.h"
#include "ltc6903.h"
#include "mc_hardware.h"

#include <errno.h>
#include <math.h>

int init_ltc6903 () {
    set_pin_ltc_cs();
    return 0;
}

int ltc6903_start () {
    uint16_t output_buf = 3;
    uint8_t array_out[2];
    array_out[0] = (output_buf >> 8) & 0xFF;
    array_out[1] = output_buf & 0xFF;
    return spi_board_device_ltc6903_tx(array_out, 2);
}

int ltc6903_set_requency (float freq, LTC6903_OUTPUT_MODE mode) {
    uint32_t oct = 0;

    if ((freq < 1.039*10e3) || (freq > 68.03*10e6)) {
        return EINVAL;
    }

    if (freq < 2.076e3) {
        oct = 0;
    } else if (freq < 4.152e3) {
        oct = 1;
    } else if (freq < 8.304e3) {
        oct = 2;
    } else if (freq < 16.61e3) {
        oct = 3;
    } else if (freq < 33.22e3) {
        oct = 4;
    } else if (freq < 66.43e3) {
        oct = 5;
    } else if (freq < 132.9e3) {
        oct = 6;
    } else if (freq < 265.7e3) {
        oct = 7;
    } else if (freq < 531.4e3) {
        oct = 8;
    } else if (freq < 1063e3) {
        oct = 9;
    } else if (freq < 2.126e6) {
        oct = 10;
    } else if (freq < 4.252e6) {
        oct = 11;
    } else if (freq < 8.503e6) {
        oct = 12;
    } else if (freq < 17.01e6) {
        oct = 13;
    } else if (freq < 34.01e6) {
        oct = 14;
    } else if (freq < 68.03e6) {
        oct = 15;
    }

    uint16_t dac;
    dac = 2048 - (2078*pow(2, 10 + oct))/freq;

    uint16_t outputBuf = oct << 12;
    outputBuf |= dac << 2;

    outputBuf |= (uint8_t)mode;

    uint8_t array_out[2];
    array_out[0] = (outputBuf >> 8) & 0xFF;
    array_out[1] = outputBuf & 0xFF;

    return spi_board_device_ltc6903_tx(array_out, 2);
}
