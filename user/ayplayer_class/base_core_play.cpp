#include "ayplayer_fat.h"
#include "base.h"

namespace AyPlayer {

/*
/// 0 данные получены.
/// 1 нет такого номера в списке.
/// -1 проблема с microsd.
int Base::getFileInfoFromListCurDir ( FILE_LIST_TYPE listType, uint32_t numberFileInList ) {
	FIL*	f = nullptr;


	 * Выбираем, какой список открыть (список должен быть сформирован заранее).

	switch( static_cast< uint32_t >( listType ) ) {
	case static_cast< uint32_t >( FILE_LIST_TYPE::NO_SORT ):
		f	=	AyPlayerFat::openFileInCurrentDir( LIST_NO_SORT_FAT_NAME );
		break;
	case static_cast< uint32_t >( FILE_LIST_TYPE::NAME_SORT ):
		f	=	AyPlayerFat::openFileInCurrentDir( LIST_SORT_NAME_FAT_NAME );
		break;
	case static_cast< uint32_t >( FILE_LIST_TYPE::LEN_SORT ):
		f	=	AyPlayerFat::openFileInCurrentDir( LIST_SORT_LEN_FAT_NAME );
		break;
	}

	if ( f == nullptr )	return -1;


	 * Смотрим, есть ли запрашиваемый номер файла в списке.

	uint32_t sizeFileList	=	f_size( f );
	uint32_t countItem	=	sizeFileList / sizeof( ItemFileInFat );
	if ( countItem <= numberFileInList ) {
		AyPlayerFat::closeFile( f );
		return 1;
	}

	FRESULT fRes;


	 * Переходим к этому файлу.

	uint32_t lseek = sizeof( ItemFileInFat ) * numberFileInList;
	fRes = f_lseek( f, lseek );
	if ( fRes != FR_OK ) {
		AyPlayerFat::closeFile( f );
		return -1;
	}

	UINT l;
	fRes = f_read( f, &this->fat.currentFileInfo, sizeof( ItemFileInFat ), &l);

	AyPlayerFat::closeFile( f );

	if ( fRes != FR_OK )
		return -1;

	if ( l != sizeof( ItemFileInFat ) )
		return -1;

	return 0;
}

/// 0 данные получены.
/// -1 проблема с microsd.
int Base::getFileCountInCurDir ( FILE_LIST_TYPE listType, uint32_t& returnCount ) {
	FIL*	f;


	 * Выбираем, какой список открыть (список должен быть сформирован заранее).

	switch( static_cast< uint32_t >( listType ) ) {
	case static_cast< uint32_t >( FILE_LIST_TYPE::NO_SORT ):
		f	=	AyPlayerFat::openFileInCurrentDir( LIST_NO_SORT_FAT_NAME );
		break;
	case static_cast< uint32_t >( FILE_LIST_TYPE::NAME_SORT ):
		f	=	AyPlayerFat::openFileInCurrentDir( LIST_SORT_NAME_FAT_NAME );
		break;
	case static_cast< uint32_t >( FILE_LIST_TYPE::LEN_SORT ):
		f	=	AyPlayerFat::openFileInCurrentDir( LIST_SORT_LEN_FAT_NAME );
		break;
	}

	if ( f == nullptr )	return -1;


	 * Смотрим, есть ли запрашиваемый номер файла в списке.

	uint32_t sizeFileList	=	f_size( f );
	returnCount	=	sizeFileList / sizeof( ItemFileInFat );

	return AyPlayerFat::closeFile( f );
}


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


 * Устанавливает в значение из текущего столбца выбранного эквалайзера в потенциометр.

void Base::setValueEqualizer (	void	) {
	switch( this->g.currentSlider ) {
	case 0:							/// A1.
		this->cfg->pcb->dp->setValue( 0, 2, this->eq.A1 );
		break;
	case 1:							/// B1.
		this->cfg->pcb->dp->setValue( 0, 0, this->eq.B1 );
		break;
	case 2:							/// C1.
		this->cfg->pcb->dp->setValue( 0, 1, this->eq.C1 );
		break;
	case 3:							/// A2.
		this->cfg->pcb->dp->setValue( 0, 3, this->eq.A2 );
		break;
	case 4:							/// B2.
		this->cfg->pcb->dp->setValue( 1, 0, this->eq.B2 );
		break;
	case 5:							/// C2.
		this->cfg->pcb->dp->setValue( 1, 1, this->eq.C2 );
		break;
	}
}

void Base::initEqualizer (	void	) {
	this->cfg->pcb->dp->setValue( 0, 2, this->eq.A1 );
	this->cfg->pcb->dp->setValue( 0, 0, this->eq.B1 );
	this->cfg->pcb->dp->setValue( 0, 1, this->eq.C1 );
	this->cfg->pcb->dp->setValue( 0, 3, this->eq.A2 );
	this->cfg->pcb->dp->setValue( 1, 0, this->eq.B2 );
	this->cfg->pcb->dp->setValue( 1, 1, this->eq.C2 );
}

void Base::volumeSet ( const uint8_t left, const uint8_t right ) {
	this->cfg->pcb->dp->setValue( 1, 2, left );			// Левый наушник.
	this->cfg->pcb->dp->setValue( 1, 3, right );			// Правый.
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


