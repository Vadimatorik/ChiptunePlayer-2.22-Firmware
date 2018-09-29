#include "gui_element_horizontal_list.h"

namespace AyPlayer {

HorizontalList::HorizontalList	(	const MakiseStyle_SMSlimHorizontalList*		style,
									MContainer*									c,
									int32_t										x,
									int32_t										y,
									uint32_t									w,
									uint32_t									h			) {
	mCreateSlimHorizontalList(	&this->shl,
								c,
								mp_rel(	x, y, w, h ),
								style	);
}

HorizontalList::~HorizontalList() {
	makise_g_cont_rem( &this->shl.el );
}

}
