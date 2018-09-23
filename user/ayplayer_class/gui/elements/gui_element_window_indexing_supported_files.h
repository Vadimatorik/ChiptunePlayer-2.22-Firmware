#pragma once

#include "nvic.h"
#include <memory>
#include "ayplayer_struct_gui_cfg.h"
#include "ayplayer_struct_pcb_cfg.h"

namespace AyPlayer {

class WindowIndexingSupportedFiles {
public:
	WindowIndexingSupportedFiles	(	const MakiseStyle_SList*		style,
										const MakiseStyle_SListItem*	itemsStyle,
										MContainer*						c,
										int32_t							x,
										int32_t							y,
										uint32_t						w,
										uint32_t						h				);

	void setActualState ( const char* const stateMessage );
	void addItem ( const char* const itemName );
	~WindowIndexingSupportedFiles();

private:
	const uint32_t cout	=	4;

private:
	MSList					sl;
	MSList_Item				slItem[ 4 ];
	char*					str;

};

}
