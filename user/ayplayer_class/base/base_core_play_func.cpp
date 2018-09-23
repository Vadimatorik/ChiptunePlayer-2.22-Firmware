#include "base.h"

namespace AyPlayer {

int Base::initMainWindow ( void ) {
	int r;

	std::shared_ptr< FIL > f;
	switch( static_cast< uint32_t >( this->lType ) ) {
	case static_cast< uint32_t >( FILE_LIST_TYPE::NO_SORT ):
		f	=	this->fat.openFile( LIST_NO_SORT_FAT_NAME, r );
		break;
	case static_cast< uint32_t >( FILE_LIST_TYPE::NAME_SORT ):
		f	=	this->fat.openFile( LIST_SORT_NAME_FAT_NAME, r );
		break;
	case static_cast< uint32_t >( FILE_LIST_TYPE::LEN_SORT ):
		f	=	this->fat.openFile( LIST_SORT_LEN_FAT_NAME, r );
		break;
	}

	if ( r != EOK )		return r;


	this->countFileInCurrentDir	= this->fat.getNumberTrackInList( f );
	this->playItem				= this->fat.readItemFileList( f, this->currentFile, r );
	if ( r != EOK )		return r;

	r = this->fat.closeFile( f );
	return r;
}

}
