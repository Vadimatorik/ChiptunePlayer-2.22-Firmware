#pragma once

#include "mc_hardware_interfaces_implementation_for_stm32_pin.h"
#include "mc_hardware_interfaces_pin_multifunc_it.h"

struct GpioPwr {					/// Питание на плате.
	McHardwareInterfaces::Pin*						const pwr5v;
	McHardwareInterfaces::Pin*						const allPwr;
};

struct GpioVolumeRocker {			/// Качелька громкости.
	McHardwareInterfaces::Pin*						const inc;
	McHardwareInterfaces::Pin*						const dec;
};

struct GpioSdControl {
	McHardwareInterfaces::Pin*						const read;
	McHardwareInterfaces::PinMultifuncIt*			const it;
	McHardwareInterfaces::Pin*						const set;
};

/// Все используемые программно выводы плеера.
struct AyplayerGpio {
	GpioPwr											pwr;
	GpioVolumeRocker								vr;
	GpioSdControl									sd;
};
