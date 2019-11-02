#include "base.h"

namespace AyPlayer {

int Base::deleteSystemFile ( void ) {
	return 0;
}

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

	/// Тут добавить условие, что если флешка не была
	/// открыта где-то во вне (не были созданы временные файлы ОС), то пересоздавать не надо. А пока пусть так.


	WindowIndexingSupportedFiles*		wisfObj	=	nullptr;
	this->gui->setWindowIndexingSupportedFiles( &wisfObj );

	int res;

	/// Составляем список файлов.
	res = this->createFileListsInSdCard( fatFsPath, wisfObj );
	if ( res != EOK ) return res;

	/// Составляем сортированные списки.
	res = this->findingFileListAndSort( fatFsPath, wisfObj );
	if ( res != EOK ) return res;

	res = this->deleteSystemFile();
	if ( res != EOK ) return res;

	return EOK;
}

}
