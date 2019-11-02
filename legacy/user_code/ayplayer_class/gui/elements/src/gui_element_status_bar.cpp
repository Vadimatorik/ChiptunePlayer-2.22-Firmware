#include "gui_element_status_bar.h"

namespace AyPlayer {

StatusBar::StatusBar	(	const MakiseStyle_SMPlayerStatusBar*	style,
							const MPlayerStatusBar_CallbackFunc* 	callbacks,
							MContainer*								c,
							int32_t									x,
							int32_t									y,
							uint32_t								w,
							uint32_t								h	) {
	m_create_player_status_bar(	&this->sb,
								c,
								mp_rel(	x, y, w, h	),
								style,
								callbacks	);
}

StatusBar::~StatusBar() {
	makise_g_cont_rem( &this->sb.e );
}

}
