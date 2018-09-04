#pragma once

#include "pin.h"

struct GpioPwr {					/// Питание на плате.
	McHardwareInterfaces::Pin*						const pwr5v;
	McHardwareInterfaces::Pin*						const allPwr;
};

struct GpioVolumeRocker {			/// Качелька громкости.
	McHardwareInterfaces::Pin*						const inc;
	McHardwareInterfaces::Pin*						const dec;
};

/// Все используемые программно выводы плеера.
struct AyplayerGpio {
	GpioPwr												pwr;
	GpioVolumeRocker									vr;
};
