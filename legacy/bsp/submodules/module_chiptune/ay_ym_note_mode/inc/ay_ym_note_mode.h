#pragma once

#ifdef MODULE_AY_YM_NOTE_MODE_ENABLED

#include "ay_ym_low_lavel.h"

//**********************************************************************
// Класс для работы с AY/YM чипами в "нотном" режиме.
//**********************************************************************
struct ay_ym_note_cfg_t {
	AyYmLowLavel*    const array_chip;
};

// Метод добавляет ноту в очередь для последующего воспроизвдения.
class AyYmNote {
public:
	constexpr AyYmNote ( const ay_ym_note_cfg_t* const cfg ) : cfg(cfg) {}

private:
    const ay_ym_note_cfg_t* const cfg;
};

#endif
