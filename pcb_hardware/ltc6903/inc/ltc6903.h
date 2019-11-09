#pragma once

typedef enum {
    LTC6903_OUTPUT_MODE_CLK_ON_INV_ON_PLUS_180 = 0,
    LTC6903_OUTPUT_MODE_CLK_OFF_INV_ON = 1,
    LTC6903_OUTPUT_MODE_CLK_ON_INV_OFF = 2,
    LTC6903_OUTPUT_MODE_CLK_OFF_INV_OFF = 3
} LTC6903_OUTPUT_MODE;

int ltc6903_init ();
int ltc6903_start ();
int ltc6903_set_requency (float freq, LTC6903_OUTPUT_MODE mode);
