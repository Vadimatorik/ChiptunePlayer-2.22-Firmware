#pragma once

#include "pin.h"

/// Комплектация выводов плеера по назначению.
struct GpioLcd {							/// LCD.
	PinBase*	const res;
	PinBase*	const dc;
	PinBase*	const cs;
};

struct GpioSpiSd {							/// SPI MicroSD.
	PinBase*	const cs;
};

struct GpioShiftRegister {					/// Расширитель портов.
	PinBase*	const st;
};

struct GpioAyChip {							/// AY/YM.
	PinBase*	const bdir;
	PinBase*	const bc1;
};

struct GpioPwr {							/// Питание на плате.
	PinBase*	const chip_1_pwr;
	PinBase*	const chip_2_pwr;
	PinBase*	const pwr_5v;
	PinBase*	const all_pwr;
};

struct GpioDigitalPotentiometer {			/// Цифровые потенциометры.
	PinBase*	const cs;
	PinBase*	const shdn;
};

struct GpioVolumeRocker {					/// Качелька громкости.
	PinBase*	const inc;
	PinBase*	const dec;
};

/// Все используемые программно выводы плеера.
struct AyplayerGpio {
	GpioLcd						lcd;
	GpioSpiSd						sd2;
	GpioShiftRegister				srSpiAudio;
	GpioAyChip					ay;
	GpioPwr						pwr;
	GpioDigitalPotentiometer		dp;
	PinBase*						buttonIn;
	GpioVolumeRocker				vr;
};
