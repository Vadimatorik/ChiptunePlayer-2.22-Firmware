#pragma once

#include "nvic.h"
#include <memory>
#include "ayplayer_struct_gui_cfg.h"
#include "ayplayer_struct_pcb_cfg.h"

namespace AyPlayer {

class StatusBar {
public:
	StatusBar	(	const MakiseStyle_SMPlayerStatusBar*	style,
					const MPlayerStatusBar_CallbackFunc* 	callbacks,
					MContainer*								c,
					int32_t									x,
					int32_t									y,
					uint32_t								w,
					uint32_t								h			);

	~StatusBar();

private:
	MPlayerStatusBar		sb;

};

}
