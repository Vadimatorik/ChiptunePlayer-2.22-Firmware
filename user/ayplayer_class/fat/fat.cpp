#include <string.h>
#include <stdio.h>

#include "fat.h"
#include "FreeRTOSConfig.h"
#include "base.h"

namespace AyPlayer {

int Fat::initFatFs ( std::shared_ptr< char >	fatStartDir ) {
	FRESULT	fr;
	fr = f_mount( &this->f, fatStartDir.get(), 1 );

	if ( fr == FRESULT::FR_OK ) {
		return EOK;
	} else {
		return EIO;
	}
}

std::shared_ptr< DIR > Fat::openDir (	std::shared_ptr< char >		path,
										int&						returnResult	) {
	FRESULT					r;
	std::shared_ptr< DIR >	d( new DIR );

	if ( d.get() == nullptr ) {
		returnResult	=	ENOMEM;
		return d;
	}

	/// Открываем директорию.
	r = f_opendir( d.get(), path.get() );

	if ( r != FRESULT::FR_OK ) {
		returnResult	=	EIO;
		return std::shared_ptr< DIR >( nullptr );
	} else {
		returnResult	=	EOK;
		return d;
	}
}

std::shared_ptr< FILINFO > Fat::readDir (	std::shared_ptr< DIR >		dir,
											int&						returnResult	) {
	FRESULT						r;
	std::shared_ptr< FILINFO >	f( new FILINFO );

	if ( f.get() == nullptr ) {
		returnResult	=	ENOMEM;
		return f;
	}

	r = f_readdir( dir.get(), f.get() );

	if ( r != FRESULT::FR_OK ) {
		returnResult	=	EIO;
		return std::shared_ptr< FILINFO >( nullptr );
	}

	returnResult	=	EOK;

	if ( f->fname[ 0 ] == 0 ) {
		return std::shared_ptr< FILINFO >( nullptr );
	}

	return f;
}

bool Fat::checkFileIsDir ( std::shared_ptr< FILINFO >	fileInfo ) {
	if ( fileInfo->fattrib & AM_DIR ) {
		return true;
	} else {
		return false;
	}
}

int Fat::closeDir(	std::shared_ptr< DIR >		dir	) {
	FRESULT			r;

	r = f_closedir( dir.get() );

	if ( r == FRESULT::FR_OK ) {
		return EOK;
	} else {
		return EIO;
	}
}


std::shared_ptr< DIR >	Fat::openDirAndFindFirstFile	(	std::shared_ptr< char >		path,
															std::shared_ptr< FILINFO >	returnFileInfo,
															std::shared_ptr< char >		maskFind,
															int&						returnResult	) {
	FRESULT				r;
	std::shared_ptr< DIR >	d( new DIR );
	if ( d.get() == nullptr ) {
		returnResult	=	ENOMEM;
		return d;
	}

	/// Открываем директорию и ищем в ней файл по маске.
	r = f_findfirst( d.get(), returnFileInfo.get(), path.get(), maskFind.get() );

	/// Этим мы указываем на то, что с самой Fat и картой все нормально.
	if ( r == FR_OK ) {
		returnResult	=	EOK;
	} else {
		returnResult	=	EIO;
		return std::shared_ptr< DIR >( nullptr );
	}

	/// Нет такого файла.
	if ( returnFileInfo->fname[ 0 ] == 0 ) {
		return std::shared_ptr< DIR >( nullptr );
	}

	return d;
}

std::shared_ptr< FILINFO >	Fat::findNextFileInDir (	std::shared_ptr< DIR >		dir,
														int&						returnResult	) {
	FRESULT						r;
	std::shared_ptr< FILINFO >	f( new FILINFO );
	if ( f.get() == nullptr ) {
		returnResult	=	ENOMEM;
		return f;
	}

	r = f_findnext( dir.get(), f.get() );

	/// Этим мы указываем на то, что с самой Fat и картой все нормально.
	if ( r == FR_OK ) {
		returnResult	=	EOK;
	} else {
		returnResult	=	EIO;
		return std::shared_ptr< FILINFO >( nullptr );
	}

	if ( f->fname[ 0 ] == 0 ) {
		return std::shared_ptr< FILINFO >( nullptr );
	}

	return f;
}

std::shared_ptr< char > Fat::getFullPath(	std::shared_ptr< char >		path,
											const char*					const fileName,
											int&						returnResult	) {
	std::shared_ptr< char > fatFsPath( new char[ MAX_PATH_FATFS_STRING_LEN + 1 ], std::default_delete< char[] >() );

	if ( fatFsPath.get() == nullptr ) {
		returnResult	=	ENOMEM;
		return fatFsPath;
	} else {
		returnResult	=	EOK;
	}

	snprintf( fatFsPath.get(), MAX_PATH_FATFS_STRING_LEN, "%s/%s", path.get(), fileName );

	return fatFsPath;
}

std::shared_ptr< FIL > Fat::openFileListWithRewrite (	std::shared_ptr< char >		fullPath,
														int&						returnResult	) {
	FRESULT				r;
	std::shared_ptr< FIL > f( new FIL );

	if ( f.get() == nullptr ) {
		returnResult	=	ENOMEM;
		return f;
	} else {
		returnResult	=	EOK;
	}

    r = f_open( f.get(), fullPath.get(), FA_CREATE_ALWAYS | FA_READ | FA_WRITE );

    if ( r != FR_OK ) {
    	returnResult	=	EIO;
    	return std::shared_ptr< FIL >( nullptr );
    }

    return f;
}

std::shared_ptr< ItemFileInFat > Fat::createtructureItemFileListFilling (	const char*			const nameTrack,
																			uint32_t			lenTickTrack,
																			AyPlayFileFormat	format,
																			int&				returnResult	) {
	std::shared_ptr< ItemFileInFat >	s( new ItemFileInFat );

	if ( s.get() == nullptr ) {
		returnResult	=	ENOMEM;
		return s;
	} else {
		returnResult	=	EOK;
	}

	/// Заполняем.
	strcpy( s->fileName, nameTrack );
	s->format		=	format;
	s->lenTick		=	lenTickTrack;

	return s;
}

int	Fat::writeFileListItem (	std::shared_ptr< FIL >				file,
								std::shared_ptr< ItemFileInFat >	item	) {
	FRESULT	r;

	/// Пробуем записать.
	UINT	l;
	r = f_write( file.get(), item.get(), sizeof( ItemFileInFat ), &l );

    /// Если записалось целиком все успешно, то возвратим 0.
    if ( ( r == FR_OK ) && ( l == sizeof( ItemFileInFat ) ) ) {
    	return EOK;
    } else {
    	return EIO;
    }
}

int Fat::closeFile (	std::shared_ptr< FIL >				file	) {
	FRESULT				r;

    r = f_close( file.get() );

    if ( r == FRESULT::FR_OK ) 	{
    	return EOK;
	} else {
		return EIO;
	}
}
























/*





FIL* Fat::openFile ( const char* const path, const char* const name ) {

	FRESULT				r;
	FIL*				f;

	/// Выделяем память под объект файла FatFS.
	f	=	( FIL* )pvPortMalloc( sizeof( FIL ) );
	assertParam( f );


	char* fullPath	=	Fat::getFullPath( path, name );
	/// Пытаемся открыть файл с перезаписью, если таковой ранее существовал.
    r = f_open( f, fullPath, FA_READ );
    vPortFree( fullPath );

    if ( r != FR_OK ) {
    	vPortFree( f );
    	f = nullptr;
    }

    return f;
}

FIL* Fat::openFileInCurrentDir ( const char* const name ) {

	FRESULT				r;
	FIL*				f;

	/// Выделяем память под объект файла FatFS.
	f	=	( FIL* )pvPortMalloc( sizeof( FIL ) );
	assertParam( f );

	uint32_t len = strlen( name ) + 2 + 1;
	char* fullName;
	fullName	=	( char* )pvPortMalloc( len );

	fullName[ 0 ]	=	'0';
	fullName[1] = ':';
	strcpy( &fullName[2], name );

	/// Пытаемся открыть файл с перезаписью, если таковой ранее существовал.
    r = f_open( f, fullName, FA_READ );
    vPortFree( fullName );

    if ( r != FR_OK ) {
    	vPortFree( f );
    	f = nullptr;
    }

    return f;
}










char* Fat::getNameTrackFromFile			( FIL* f, uint32_t nubmerTrack ) {
	FRESULT				r;

	const uint32_t	lseek	=	sizeof( ItemFileInFat ) * nubmerTrack;
	r	=	f_lseek( f, lseek );

	if ( r != FR_OK ) {
		return nullptr;
	}

	char*	name			=	( char* )pvPortMalloc( FF_MAX_LFN + 1 );
	UINT		l;
	r	=	f_read( f, name, FF_MAX_LFN + 1, &l );
	if ( ( r == FR_OK ) && ( l == FF_MAX_LFN + 1 ) ) {
		return name;
	} else {
		vPortFree( name );
		return nullptr;
	}
}

uint32_t Fat::getSizeTrackFromFile		( FIL* f, uint32_t nubmerTrack ) {
	FRESULT				r;

	const uint32_t	lseek	=	( sizeof( ItemFileInFat ) * nubmerTrack ) + FF_MAX_LFN + 1 + sizeof( AyPlayFileFormat );
	r	=	f_lseek( f, lseek );

	if ( r != FR_OK )
		return 0xFFFFFFFF;

	uint32_t	len;
	UINT		l;
	r	=	f_read( f, &len, sizeof( uint32_t ), &l );

	return ( ( r == FR_OK ) && ( l == sizeof( uint32_t ) ) ) ? len : 0xFFFFFFFF;
}


int Fat::setOffsetByteInOpenFile ( FIL* f, uint32_t offset ) {
	if ( f_lseek( f, offset ) == FR_OK ) {
		return 0;
	} else {
		return -1;
	}
}

int Fat::readFromOpenFile ( FIL* f, uint8_t* returnData, const uint32_t countByte ) {
	UINT		l;
	FRESULT		r;

	r = f_read( f, returnData, static_cast< UINT >( countByte ), &l );

	if ( r != FR_OK )
		return -1;

	if ( l != countByte )
		return -2;

	return 0;
}

int Fat::getSizeFromOpenTreck ( FIL* f, uint32_t& returnSizeByte ) {
	returnSizeByte = f_size( f );
	return 0;
}

int Fat::readItemFileListAndRemoveItem ( FIL* f, ItemFileInFat* item, uint32_t numberTrack ) {
	FRESULT				r;

	const uint32_t	lseek	=	sizeof( ItemFileInFat ) * numberTrack;
	r	=	f_lseek( f, lseek );

	if ( r != FR_OK ) {
		return -1;
	}

	UINT		l;
	r	=	f_read( f, item, sizeof( ItemFileInFat ), &l );
	return ( ( r == FR_OK ) && ( l == sizeof( ItemFileInFat ) ) ) ? 0 : -1;
}

int Fat::checkingFileOrDir ( const char* path, const char* nameFile, FILINFO* fi, FRESULT& fatReturn ) {
	FRESULT		r;

	char* fullPath;
	fullPath = Fat::getFullPath( path, nameFile );
	r = f_stat( fullPath, fi );
	vPortFree( fullPath );

	switch( ( uint32_t )r ) {
	case ( uint32_t )FR_OK:			return 1;
	case ( uint32_t )FR_NO_FILE:	return 0;
	default:
		fatReturn = r;
		return -1;
	}
}

/// 0 - ок, 1 - нет файла, -1 флешка проблемная.
int Fat::removeFile ( const char* path, const char* nameFile, FRESULT& fatReturn ) {
	FRESULT		r;

	char* fullPath;
	fullPath = Fat::getFullPath( path, nameFile );

	do {
		r = f_chmod( fullPath, 0, AM_RDO|AM_ARC|AM_SYS|AM_HID );		/// Снимаем блокировки.
		if ( r != FR_OK ) break;
		r = f_unlink( fullPath );
	} while( false );

	vPortFree( fullPath );

	switch( ( uint32_t )r ) {
	case ( uint32_t )FR_OK:			return 0;
	case ( uint32_t )FR_NO_FILE:	return 1;
	default:
		fatReturn = r;
		return -1;
	}
}

int Fat::removeDir ( const char* path, FRESULT& fatReturn ) {
	FRESULT		r;
	do {
		r = f_chmod( path, 0, AM_RDO|AM_ARC|AM_SYS|AM_HID );		/// Снимаем блокировки.
		if ( r != FR_OK ) break;
		r = f_unlink( path );
	} while( false );

	switch( ( uint32_t )r ) {
	case ( uint32_t )FR_OK:			return 0;
	case ( uint32_t )FR_NO_FILE:	return 1;
	default:
		fatReturn = r;
		return -1;
	}
}

int Fat::removeDirRecurisve ( char* fullPath, FRESULT& fatReturn ) {
	( void )fullPath;
	( void )fatReturn;
	static FILINFO			f;
	static int				r = 0;

	DIR*	d	=	Fat::openDir( fullPath );

	/// Что-то не так с драйверами или директории не существует.
	if ( d == nullptr )
		return -1;

	/// Рекурсивно обходим все папки.
	while( 1 ) {
		fatReturn = f_readdir( d, &f );

		/// Если проблемы на нижнем уравне.
		if ( fatReturn != FR_OK ) {
			r = -1;
			break;
		}

		/// Закончились элементы в текущей директории.
		if ( f.fname[ 0 ] == 0 )
			break;

		/// Найдена новая директория.
		if ( f.fattrib & AM_DIR ) {
			uint32_t i = strlen( fullPath );
			sprintf( &fullPath[ i ], "/%s", f.fname );

			r	=	Fat::removeDirRecurisve( fullPath, fatReturn );

			if ( fatReturn != FRESULT::FR_OK ) {								/// Аварийная ситуация.
				r = -1;
				break;
			}

			fullPath[ i ] = 0;
		} else {
			r = Fat::removeFile( fullPath, f.fname, fatReturn );
			if ( r != 0 )
				break;
		}
	}

	/// Фиксируем FRESULT::FR_DISK_ERR как приоритет над всем.
	r = ( Fat::closeDir( d ) == -1 ) ? -1 : r;

	/// Удалили все в директории можно и ее саму.
	if ( r == 0 )
		r = Fat::removeDir( fullPath, fatReturn );

	return r;
	return 0;
}*/

}

