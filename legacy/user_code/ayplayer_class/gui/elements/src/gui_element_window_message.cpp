#include "gui_element_window_message.h"

namespace AyPlayer {

WindowMessage::WindowMessage(	const char*						const message,
								const MakiseStyle_SMessageWindow*		style,
								MContainer*						c,
								int32_t							x,
								int32_t							y,
								uint32_t						w,
								uint32_t						h				) {
	uint32_t		l	=	strlen( message );
	this->str			=	new char[ l + 1 ];
	this->str[ l ]		=	0;

	strcpy( this->str, message );

	m_create_message_window(	&this->mw,
								c,
								mp_rel( x, y, w, h ),
								this->str,
								style );
}

WindowMessage::~WindowMessage() {
	delete this->str;
	makise_g_cont_rem( &this->mw.el );
}

}
