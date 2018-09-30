#include "base.h"

namespace AyPlayer {

int Base::initMainWindow ( void ) {
	int r;

	switch( static_cast< uint32_t >( this->lType ) ) {
	case static_cast< uint32_t >( FILE_LIST_TYPE::NO_SORT ):
		this->playList	=	this->fat.openFile( LIST_NO_SORT_FAT_NAME, r );
		break;
	case static_cast< uint32_t >( FILE_LIST_TYPE::NAME_SORT ):
		this->playList	=	this->fat.openFile( LIST_SORT_NAME_FAT_NAME, r );
		break;
	case static_cast< uint32_t >( FILE_LIST_TYPE::LEN_SORT ):
		this->playList	=	this->fat.openFile( LIST_SORT_LEN_FAT_NAME, r );
		break;
	}

	if ( r != EOK )		return r;

	this->countFileInCurrentDir	= this->fat.getNumberTrackInList( this->playList );
	this->playItem				= this->fat.readItemFileList( this->playList, this->currentFile, r );
	if ( r != EOK )		return r;

	uint32_t countTrecks;
	countTrecks = this->fat.getNumberTrackInList( this->playList );

	this->gui->setWindowMain( this->playItem, countTrecks );
	this->gui->update();

	return EOK;
}

}
