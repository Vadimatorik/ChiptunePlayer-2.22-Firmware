#pragma once

#include "ayplayer_pcb_hardware.h"
#include "ayplayer_mc_hardware.h"

struct AyPlayerPcbStrcut {
    MicrosdBase *const sd;
    MonoLcd::ST7565 *const lcd;
    WaveGenerators::LTC6903 *const generator;
    button::base *const buttons;
};
