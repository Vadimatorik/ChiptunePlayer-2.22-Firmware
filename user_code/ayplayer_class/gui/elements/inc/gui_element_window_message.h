#include "nvic.h"
#include <memory>
#include "ayplayer_struct_gui_cfg.h"
#include "ayplayer_struct_pcb_cfg.h"

namespace AyPlayer {

class WindowMessage {
public:
	WindowMessage (	const char*								const message,
					const MakiseStyle_SMessageWindow*		style,
					MContainer*								c,
					int32_t									x,
					int32_t									y,
					uint32_t								w,
					uint32_t								h			);

	~WindowMessage();

private:
	MMessageWindow					mw;
	char*							str;

};

}
