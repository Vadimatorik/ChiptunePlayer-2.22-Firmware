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



/// Метод вызывается только из main окна.
void Base::stopPlayFile ( void ) {
	this->cfg->ay->stop();
	this->playState		=	AYPLAYER_STATUS::STOP;
	mPlayBarResetTrack( &this->g.pb );
}

/// Метод вызывается только из main окна.
void Base::playPauseSet( bool state ) {
	this->cfg->ay->setPause( state );

	if ( state ) {
		this->playState		=	AYPLAYER_STATUS::PLAY;
	} else {
		this->playState		=	AYPLAYER_STATUS::PAUSE;
	}
}

void Base::trackMainWindowInit ( void ) {
	USER_OS_STATIC_TIMER_STOP( this->timNameScroll );
	USER_OS_STATIC_TIMER_RESET( this->timNameScroll );
	USER_OS_STATIC_TIMER_CHANGE_PERIOD( this->timNameScroll, SCROLL_STRING_NAME_LOW );

	/// Имя текущему элементу.
	mSlimHorizontalListSetStringCurrentItem( &this->g.shl, this->fat.currentFileInfo.fileName );

	/// Указываем колличество элементов в директории.

	mPlayBarSetNewTrack( &this->g.pb, this->fat.currentFileInfo.lenTick / 50 );

	USER_OS_STATIC_TIMER_START( this->timNameScroll );
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


