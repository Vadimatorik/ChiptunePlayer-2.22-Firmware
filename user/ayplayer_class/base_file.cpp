#include "base.h"

namespace AyPlayer {

int Base::initFileLists ( void ) {
	std::shared_ptr< char > fatFspath( new char[ 1024 ], std::default_delete< char[] >() );
	if ( fatFspath.get() == nullptr )	return ENOMEM;

	/// Начинаем поиск с корневого каталога.
	strcpy( fatFspath.get(), "0:" );
	this->printMessage( RTL_TYPE_M::RUN_MESSAGE_OK, "Init file lists!" );

	FRESULT	fr;
	fr = f_mount( &this->fat.f, fatFspath.get(), 1 );
	fatFsCheckAndReturn( fr );

	return EOK;
}

}
