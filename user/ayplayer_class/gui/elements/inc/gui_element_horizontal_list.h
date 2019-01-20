#pragma once

#include "nvic.h"
#include <memory>
#include "ayplayer_struct_gui_cfg.h"
#include "ayplayer_struct_pcb_cfg.h"

namespace AyPlayer {

class HorizontalList {
public:
	HorizontalList	(	const MakiseStyle_SMSlimHorizontalList*		style,
						MContainer*									c,
						int32_t										x,
						int32_t										y,
						uint32_t									w,
						uint32_t									h	);

	~HorizontalList();

public:
	void	setStringCurrentItem		(	const char*		string		);
	void	setItemCount				(	uint32_t		countItems	);
	int		scroll 						(	void	);

private:
	MSlimHorizontalList					shl;

};

}
