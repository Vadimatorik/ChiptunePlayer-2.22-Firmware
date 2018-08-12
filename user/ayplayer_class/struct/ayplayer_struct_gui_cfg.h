#pragma once

#include "play_bar.h"
#include "play_list.h"
#include "player_status_bar.h"
#include "progress_bar.h"
#include "makise_e_slist.h"
#include "scroll_string.h"
#include "slim_horizontal_list.h"

struct AyPlayerGuiModuleStyleCfg {
	const MakiseStyle_SMessageWindow			smw;
	const MakiseStyle_SList						ssl;
	const MakiseStyle_SListItem					sslItem;
	const MakiseStyle_SMPlayerStatusBar			statusBarCfg;
	const MPlayerStatusBar_CallbackFunc			statusBarCallbackCfg;
	const MakiseStyle_PlayBar					playBarStyle;
	const MakiseStyle_SMSlimHorizontalList		horizontalListStyle;
	const MakiseStyle							m;
};

struct AyPlayerModuleGui {
	MMessageWindow								mw;
	MSList										sl;
	MSList_Item*								slItem[ 4 ];
	MPlayerStatusBar							sb;
	MPlayBar									pb;
	MSlimHorizontalList							shl;
	MSlider*									sliders[ 6 ];
	int32_t										currentSlider;
};
