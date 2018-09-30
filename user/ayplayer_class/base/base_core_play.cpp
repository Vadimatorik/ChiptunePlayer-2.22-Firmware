#include "base.h"
#include "fat/fat.h"

namespace AyPlayer {

/*
int Base::startPlayFile ( void ) {
	this->cfg->pcb->dp->connectOn();



	this->initEqualizer();

	this->volumeSet(	this->volumeTable[ this->currentVolumeIndex ],
						this->volumeTable[ this->currentVolumeIndex ]	);

	int r = 0;
	switch( static_cast< uint32_t >( this->fat.currentFileInfo.format ) ) {
	case static_cast< uint32_t >( AyPlayFileFormat::psg ):
		this->cfg->ay->setPlayFileName( this->fat.currentFileInfo.fileName );
		this->cfg->ay->setUsingChip( 1 );
		r	=	this->cfg->ay->psgFilePlay();
	}

 	return r;
}





void Base::startPlayTrack ( void ) {
	int r;
	r = this->getFileInfoFromListCurDir( this->lType, this->currentFile );
	if ( r != 0 )
		return;

	if ( this->wNow == AYPLAYER_WINDOW_NOW::MAIN ) {
		this->trackMainWindowInit();
	}

	USER_OS_GIVE_BIN_SEMAPHORE( this->cfg->os->sStartPlay );
}

*/

}


