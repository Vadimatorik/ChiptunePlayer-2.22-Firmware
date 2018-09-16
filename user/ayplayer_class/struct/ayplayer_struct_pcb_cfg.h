#pragma once

#include "ayplayer_pcb_hardware.h"
#include "ayplayer_mc_hardware.h"

struct AyPlayerPcbStrcut {
	DigitalPotentiometer::AD5204*		const	dp;
	MicrosdBase* 						const	sd;
	MonoLcd::ST7565*					const	lcd;
	WaveGenerators::LTC6903*			const	generator;
};
