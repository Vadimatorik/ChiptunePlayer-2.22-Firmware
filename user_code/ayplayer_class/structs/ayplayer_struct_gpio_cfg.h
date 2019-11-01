#pragma once

#include "pin.h"
#include "pin_multifunc_it.h"

struct GpioPwr {					/// Питание на плате.
	mc_interfaces::Pin*						const pwr5v;
	mc_interfaces::Pin*						const allPwr;
};

struct GpioVolumeRocker {			/// Качелька громкости.
	mc_interfaces::Pin*						const inc;
	mc_interfaces::Pin*						const dec;
};

struct GpioSdControl {
	mc_interfaces::Pin*						const read;
	mc_interfaces::PinMultifuncIt*			const it;
	mc_interfaces::Pin*						const set;
};

/// Все используемые программно выводы плеера.
struct AyplayerGpio {
	GpioPwr											pwr;
	GpioVolumeRocker								vr;
	GpioSdControl									sd;
};
