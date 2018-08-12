#pragma once

#include "shift_register.h"
#include "shift_register_8bit_port.h"
#include "shift_register_pin.h"
#include "microsd_card_sdio.h"
#include "mono_lcd_lib_st7565.h"
#include "module_digital_potentiometer_AD5204.h"
#include "ay_ym_low_lavel.h"
#include "ay_ym_file_play.h"

extern ShiftRegister			boardShiftRegisters;
extern MicrosdSdio				sd;
extern ST7565					lcd;
extern AD5204					soundDp;
extern AyYmLowLavel				ayLow;

extern ShiftRegisterPin			pwrAy1;
extern ShiftRegisterPin			pwrAy2;
extern ShiftRegisterPin			ay1Res;
extern ShiftRegisterPin			ay2Res;
extern ShiftRegisterPin			bc1;
extern ShiftRegisterPin			bdir;

extern ShiftRegisterPort8bit	ayPorts[ 2 ];
