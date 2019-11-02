#pragma once

#include "ayplayer_mc_hardware.h"

namespace AyPlayer {

class SdControl {
public:
	SdControl	(	mc_interfaces::PinMultifuncIt*	pinIt,
					mc_interfaces::Pin*				pinRead,
					mc_interfaces::Pin*				pinSet	);

public:
	void	init					(	void	);
	bool	getState				(	void	);

private:
	mc_interfaces::PinMultifuncIt*	pinIt;
	mc_interfaces::Pin*				pinRead;
	mc_interfaces::Pin*				pinSet;

};

}
