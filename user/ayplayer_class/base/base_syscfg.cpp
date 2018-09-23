#include "ayplayer_button.h"
#include "base.h"
#include "user_os.h"

namespace AyPlayer {

int Base::setSyscfgDefaultGuiParams ( void ) {
	this->gui->setMaxIlluminationDuty( 0.8 );
	this->gui->setMinIlluminationDuty( 0.3 );
	this->gui->setMaxIlluminationTime( 10 );
	this->gui->setMinIlluminationTime( 5 );

	return EOK;
}

/// Потом надо будет сделать чтение с флешки или из настроек по умолчанию.
int Base::setSysCfgParams ( void ) {
	this->gui->setMaxIlluminationDuty( 0.8 );
	this->gui->setMinIlluminationDuty( 0.3 );
	this->gui->setMaxIlluminationTime( 5 );
	this->gui->setMinIlluminationTime( 2 );

	this->lType			=	FILE_LIST_TYPE::NAME_SORT;
	this->playState		=	AYPLAYER_STATUS::STOP;
	this->currentFile	=	0;
	this->wNow			=	AYPLAYER_WINDOW_NOW::MAIN;

	int r;

	r = this->fat.setCurrentDir( "0:" );
	if ( r != EOK )		return r;

	return EOK;
}

}
