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

	/// Тут добавить условие, что если флешка не была
	/// открыта где-то во вне (не были созданы временные файлы ОС), то пересоздавать не надо. А пока пусть так.


	std::shared_ptr< WindowIndexingSupportedFiles > w( this->gui->addWindowIndexingSupportedFiles() );

	int res;

	/// Составляем список файлов.
	res = this->createFileListsInSdCard( fatFsPath, w.get() );
	if ( res != EOK ) return res;

	/// Составляем сортированные списки.
	res = this->findingFileListAndSort( fatFsPath, w.get() );
	if ( res != EOK ) return res;

	return EOK;
}

}
