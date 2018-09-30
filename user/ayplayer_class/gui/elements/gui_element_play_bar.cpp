#include "gui_element_play_bar.h"

namespace AyPlayer {

PlayBar::PlayBar	(	const MakiseStyle_PlayBar*				style,
						MContainer*								c,
						int32_t									x,
						int32_t									y,
						uint32_t								w,
						uint32_t								h			) {
	mCreatePlayBar(	&this->pb,
					c,
					mp_rel(	x, y, w, h	),
					1,
					style );
}

void PlayBar::setLenInTick	(	uint32_t	 		quantityTicks	) {
	mPlayBarSetNewTrack( &this->pb, quantityTicks / 50 );
}

void	PlayBar::reset	(	void	) {
	mPlayBarResetTrack( &this->pb );
}

PlayBar::~PlayBar () {
	makise_g_cont_rem( &this->pb.el );
}

}
