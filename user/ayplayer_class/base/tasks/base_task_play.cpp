#include "base.h"

namespace AyPlayer {

void Base::playTask (  void* obj  ) {
	(void)obj;
	/*
	Base* o =( Base* ) obj;
	int  r;
	while ( true ) {
		USER_OS_TAKE_BIN_SEMAPHORE( o->cfg->os->sStartPlay, portMAX_DELAY );
		o->playState		=	AYPLAYER_STATUS::PLAY;
		//o->guiUpdate();
		r	=	o->startPlayFile();

		/// Если трек был остановлен или проблемы на аппаратном уровне.
		if ( r != 0 ) {
			o->playState		=	AYPLAYER_STATUS::STOP;
			continue;
		}

		/// Переходим на следующий трек если это был не последний.
		if ( o->currentFile < o->countFileInCurrentDir - 1 ) {
			o->currentFile++;
		} else {
			o->currentFile	=	0;
		}
		o->startPlayTrack();
	}*/
}

}
