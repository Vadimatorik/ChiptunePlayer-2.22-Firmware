#include "ayplayer_button.h"
#include "base.h"
#include "user_os.h"

namespace AyPlayer {

/// Потом надо будет сделать чтение с флешки или из настроек по умолчанию.
void Base::setSysCfgParams ( void ) {
	this->gui->setMaxIlluminationDuty( 0.8 );
	this->gui->setMinIlluminationDuty( 0.3 );
	this->gui->setMaxIlluminationTime( 5 );
	this->gui->setMinIlluminationTime( 2 );

	this->lType			=	FILE_LIST_TYPE::NAME_SORT;
	this->playState		=	AYPLAYER_STATUS::STOP;
	this->currentFile	=	0;
	this->wNow			=	AYPLAYER_WINDOW_NOW::MAIN;
}

}
