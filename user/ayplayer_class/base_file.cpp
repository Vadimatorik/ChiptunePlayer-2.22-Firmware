#include "base.h"

namespace AyPlayer {

int Base::initFileLists ( void ) {
	std::shared_ptr< char > fatFsPath( new char[ MAX_PATH_FATFS_STRING_LEN + 1 ], std::default_delete< char[] >() );
	if ( fatFsPath.get() == nullptr )	return ENOMEM;

	/// Начинаем поиск с корневого каталога.
	strcpy( fatFsPath.get(), "0:" );
	this->printMessage( RTL_TYPE_M::RUN_MESSAGE_OK, "Init file lists!" );

	/// Пытаемся открыть microsd карту.
	if ( this->fat.initFatFs( fatFsPath ) != EOK ) {
		this->printMessage( RTL_TYPE_M::RUN_MESSAGE_OK, "FatFs in Micro SD card was not finded!" );
		return EIO;
	}

	this->printMessage( RTL_TYPE_M::RUN_MESSAGE_OK, "FatFs in Micro SD card successfully finded!" );

	/// Составляем список файлов.
	this->createFileListsInSdCard( fatFsPath );

	return EOK;
}

}
