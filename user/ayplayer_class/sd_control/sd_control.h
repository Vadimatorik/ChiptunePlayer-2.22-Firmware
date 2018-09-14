#pragma once

#include "ayplayer_mc_hardware.h"

namespace AyPlayer {

class SdControl {
public:
	SdControl	(	McHardwareInterfaces::PinMultifuncIt*	pinIt,
					McHardwareInterfaces::Pin*				pinRead,
					McHardwareInterfaces::Pin*				pinSet	);

public:
	void	init					(	void	);
	bool	getState				(	void	);

private:
	McHardwareInterfaces::PinMultifuncIt*	pinIt;
	McHardwareInterfaces::Pin*				pinRead;
	McHardwareInterfaces::Pin*				pinSet;

};

}
