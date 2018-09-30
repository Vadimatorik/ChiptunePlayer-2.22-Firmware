#pragma once

#include "ayplayer_pcb_hardware.h"

namespace AyPlayer {

struct MuxerChannelCfg {
	uint8_t				chipNumber;
	uint8_t				channelNumber;
	uint8_t				valueDefault;
};

struct MuxerCfg {
	DigitalPotentiometer::AD5204*		dp;
	MuxerChannelCfg						left;
	MuxerChannelCfg						right;
	MuxerChannelCfg						A1;
	MuxerChannelCfg						B1;
	MuxerChannelCfg						C1;
	MuxerChannelCfg						A2;
	MuxerChannelCfg						B2;
	MuxerChannelCfg						C2;
};


class Muxer {
public:
	Muxer	(	const MuxerCfg*		const cfg	);

public:
	int		init					(	void	);
	int		setVolume				(	uint8_t		volume	);

private:
	const MuxerCfg*			const cfg;

};

};
