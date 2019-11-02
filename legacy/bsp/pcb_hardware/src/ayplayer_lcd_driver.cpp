#include "ayplayer_mc_hardware.h"
#include "mono_lcd_lib_st7565.h"

const mono_lcd::st7565_cfg st7565Cfg = {
    .a0        =    &lcdDc,
    .res    =    &lcdRes,
    .cs        =    &lcdCs,
    .s        =    &lcdSpi,
    .mode    =    mono_lcd::st7565_mode::standard
};

/// Для доступа MakiseGUI.
extern "C" {
uint8_t makiseLcdBuf[1024] = {0};
}

mono_lcd::st7565 lcd(&st7565Cfg, makiseLcdBuf);
