#pragma once

#include "nvic.h"
#include <memory>
#include "ayplayer_struct_gui_cfg.h"
#include "ayplayer_struct_pcb_cfg.h"

namespace AyPlayer {

class PlayBar {
public:
	PlayBar	(	const MakiseStyle_PlayBar*				style,
				MContainer*								c,
				int32_t									x,
				int32_t									y,
				uint32_t								w,
				uint32_t								h			);

	~PlayBar();

public:
	void	setLenInTick			(	uint32_t	 		quantityTicks	);

private:
	MPlayBar				pb;

};

}
