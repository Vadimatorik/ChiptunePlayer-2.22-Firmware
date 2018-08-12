#include "ayplayer_mc_hardware.h"
#include "mono_lcd_lib_st7565.h"

const ST7565Cfg st7565Cfg = {
	.a0		=	&lcdDc,
	.res	=	&lcdRes,
	.cs		=	&lcdCs,
	.s		=	&lcdSpi,
	.mode	=	ST7565_MODE::STANDARD
};

/// Для доступа MakiseGUI.
extern "C" {
uint8_t makiseLcdBuf[ 1024 ] = { 0 };
}

ST7565 lcd( &st7565Cfg, makiseLcdBuf );
