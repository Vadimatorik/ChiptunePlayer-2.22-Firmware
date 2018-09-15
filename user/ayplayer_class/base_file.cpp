#include "base.h"

namespace AyPlayer {

int Base::initFileLists ( void ) {
	std::shared_ptr< char > fatFspath( new char[ 1024 ], std::default_delete< char[] >() );
	if ( fatFspath.get() == nullptr )	return ENOMEM;

	/// Начинаем поиск с корневого каталога.
	strcpy( fatFspath.get(), "0:" );
	this->printMessage( RTL_TYPE_M::RUN_MESSAGE_OK, "Init file lists!" );

	/// Пытаемся открыть microsd карту.
	FRESULT	fr;
	fr = f_mount( &this->fat.f, fatFspath.get(), 1 );

	if ( fr != FRESULT::FR_OK ) {
		this->printMessage( RTL_TYPE_M::RUN_MESSAGE_OK, "FatFs in Micro SD card was not finded!" );
		return -1;
	}

	this->printMessage( RTL_TYPE_M::RUN_MESSAGE_OK, "FatFs in Micro SD card successfully finded!" );

	return EOK;
}

}
