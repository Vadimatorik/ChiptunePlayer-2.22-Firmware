#include "ayplayer.h"

extern "C" {

extern MHost				makiseHost;

}

namespace AyPlayer {
void AyPlayer::waitSdCardDisconnect ( void ) {
	while( true ) {
		if ( this->cfg->pcb->sd->getStatus() != EC_SD_STATUS::OK )
			break;
	}

	char	massage[] = "SD was been disconnect.";

	this->cfg->l->sendMessage( RTL_TYPE_M::INIT_OK, massage );

}

}
