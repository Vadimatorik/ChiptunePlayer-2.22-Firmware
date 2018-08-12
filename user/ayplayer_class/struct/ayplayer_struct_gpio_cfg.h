#pragma once

#include "pin.h"

struct GpioPwr {					/// Питание на плате.
	PinBase*						const pwr5v;
	PinBase*						const allPwr;
};

struct GpioVolumeRocker {			/// Качелька громкости.
	PinBase*						const inc;
	PinBase*						const dec;
};

/// Все используемые программно выводы плеера.
struct AyplayerGpio {
	GpioPwr							pwr;
	GpioVolumeRocker				vr;
};
