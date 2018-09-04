#pragma once

#include "ayplayer_pcb_hardware.h"
#include "ayplayer_mc_hardware.h"

struct AyPlayerPcbStrcut {
	DigitalPotiometer::AD5204*			const dp;
	MicrosdBase* 						const sd;
};
