#pragma once

#include <stdint.h>

#define IMG_STOP_WIDTH 7
#define IMG_STOP_HEIGHT 7

const uint8_t img_stop_bits[] = {
    0x7f, 0x41, 0x5d, 0x5d, 0x5d, 0x41, 0x7f
};
