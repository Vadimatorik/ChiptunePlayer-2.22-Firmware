#include <string.h>
#include <stdio.h>

#include "fat.h"
#include "FreeRTOSConfig.h"
#include "base.h"

namespace AyPlayer {

int Fat::initFatFs ( std::shared_ptr< char >	fatStartDir ) {
	FRESULT	fr;

	static FATFS				f;

	fr = f_mount( &f, fatStartDir.get(), 1 );

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

int Fat::setCurrentDir ( const char* const path ) {
	FRESULT	r;
	r	=	f_chdir( path );

	if ( r == FRESULT::FR_OK ) {
		return EOK;
	} else {
		return EIO;
	}

	return EIO;
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

std::shared_ptr< FIL > Fat::openFileListWithRewrite (	std::shared_ptr< char >		path,
														const char*					const fileName,
														int&						returnResult	) {
	std::shared_ptr< char >	fullPath( this->getFullPath( path, fileName, returnResult ) );
	if ( returnResult != EOK ) {
		returnResult = ENOMEM;
		return std::shared_ptr< FIL >( nullptr );
	}

	return this->openFileListWithRewrite( fullPath, returnResult );
}

std::shared_ptr< FIL > Fat::openFileListWithRewrite (	std::shared_ptr< char >		fullPath,
														int&						returnResult	) {
	FRESULT					r;
	std::shared_ptr< FIL >	f( new FIL );

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

	memset( s->fileName, 0, FF_MAX_LFN + 1 );

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


std::shared_ptr< FIL >	Fat::openFile							(	std::shared_ptr< char >		fullPath,
																	int&						returnResult	) {
	FRESULT					r;
	std::shared_ptr< FIL >	f( new FIL );

	if ( f.get() == nullptr ) {
		returnResult	=	ENOMEM;
		return f;
	} else {
		returnResult	=	EOK;
	}

    r = f_open( f.get(), fullPath.get(), FA_READ );

    if ( r != FR_OK ) {
       	returnResult	=	EIO;
       	return std::shared_ptr< FIL >( nullptr );
    }

    return f;
}

std::shared_ptr< FIL >	Fat::openFile							(	const char*					const path,
																	int&						returnResult	) {
	FRESULT					r;
	std::shared_ptr< FIL >	f( new FIL );

	if ( f.get() == nullptr ) {
		returnResult	=	ENOMEM;
		return f;
	} else {
		returnResult	=	EOK;
	}

    r = f_open( f.get(), path, FA_READ );

    if ( r != FR_OK ) {
       	returnResult	=	EIO;
       	return std::shared_ptr< FIL >( nullptr );
    }

    return f;
}

std::shared_ptr< FIL >	Fat::openFile							(	std::shared_ptr< char >		path,
																	const char*					const name,
																	int&						returnResult	) {
	std::shared_ptr< char >	fullPath( this->getFullPath( path, name, returnResult ) );
	if ( returnResult != EOK ) {
		returnResult = ENOMEM;
		return std::shared_ptr< FIL >( nullptr );
	}

	return this->openFile( fullPath, returnResult );
}

uint32_t Fat::getFileSize ( std::shared_ptr< FIL >	file ) {
	return f_size( file );
}

int Fat::readFromFile (	std::shared_ptr< FIL >		file,
						uint8_t*					returnDataArray,
						uint32_t					countReadByte ) {
	UINT		l;
	FRESULT		r;

	r = f_read( file.get(), returnDataArray, static_cast< UINT >( countReadByte ), &l );

	if ( r != FRESULT::FR_OK ) {
		return EIO;
	}

	if ( l != countReadByte ) {
		return EINVAL;
	}

	return EOK;
}


int Fat::setOffsetByteInFile (	std::shared_ptr< FIL >		file,
								uint32_t					offset	) {
	if ( f_lseek( file.get(), offset ) == FR_OK ) {
		return EOK;
	} else {
		return EIO;
	}
}


std::shared_ptr< char > Fat::getNameTrackFromFile			(	std::shared_ptr< FIL >		listFile,
																uint32_t					nubmerTrack,
																int&						returnResult	) {
	FRESULT				r;

	const uint32_t	lseek	=	sizeof( ItemFileInFat ) * nubmerTrack;
	r	=	f_lseek( listFile.get(), lseek );

	if ( r != FR_OK ) {
		returnResult	=	EIO;
		return std::shared_ptr< char >( nullptr );
	}

	std::shared_ptr< char > name( new char[ FF_MAX_LFN + 1 ], std::default_delete< char[] >() );

	if ( name.get() == nullptr ) {
		returnResult	=	ENOMEM;
		return name;
	}

	UINT		l;
	r	=	f_read( listFile.get(), name.get(), FF_MAX_LFN + 1, &l );
	if ( ( r == FR_OK ) && ( l == FF_MAX_LFN + 1 ) ) {
		returnResult	=	EOK;
		return name;
	} else {
		returnResult	=	EIO;
		return std::shared_ptr< char >( nullptr );
	}
}

uint32_t Fat::getNumberTrackInList			(	std::shared_ptr< FIL >		listFile	) {
	uint32_t size = this->getFileSize( listFile );
	uint32_t numberTracks	=	size / sizeof( ItemFileInFat );
	return numberTracks;
}

uint32_t Fat::getSizeTrackFromFile		(	std::shared_ptr< FIL >		listFile,
											uint32_t					nubmerTrack,
											int&						returnResult ) {
	FRESULT				r;

	const uint32_t	lseek	=	( sizeof( ItemFileInFat ) * nubmerTrack ) + FF_MAX_LFN + 1 + sizeof( AyPlayFileFormat );
	r	=	f_lseek( listFile.get(), lseek );

	if ( r != FR_OK ) {
		returnResult = EIO;
		return 0;
	}

	uint32_t	len;
	UINT		l;
	r	=	f_read( listFile.get(), &len, sizeof( uint32_t ), &l );

	if ( !( ( r == FR_OK ) && ( l == sizeof( uint32_t ) ) ) ) {
		returnResult = EIO;
	}

	return r;
}

std::shared_ptr< ItemFileInFat > Fat::readItemFileList (	std::shared_ptr< FIL >				f,
															uint32_t numberTrack,
															int&						returnResult	) {
	FRESULT				r;

	std::shared_ptr< ItemFileInFat >	item( new ItemFileInFat );
	if ( item.get() == nullptr ) {
			returnResult	=	ENOMEM;
			return item;
		} else {
			returnResult	=	EOK;
		}

	const uint32_t	lseek	=	sizeof( ItemFileInFat ) * numberTrack;
	r	=	f_lseek( f.get(), lseek );

	if ( r != FR_OK ) {
		returnResult =  EIO;
		std::shared_ptr< ItemFileInFat >	item( nullptr );
	}

	UINT		l;
	r	=	f_read( f.get(), item.get(), sizeof( ItemFileInFat ), &l );

	if ( ( r == FR_OK ) && ( l == sizeof( ItemFileInFat ) ) ) {
		returnResult = EOK;
		return item;
	}

	returnResult = EIO;
	return std::shared_ptr< ItemFileInFat >( nullptr );
}

/// 1
/*
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
}*/

/// 0 - ок, 1 - нет файла, -1 флешка проблемная.
int Fat::removeFile ( std::shared_ptr< char > path, const char* nameFile, FRESULT& fatReturn ) {
	int int_r;
	std::shared_ptr< char >	fullPath( this->getFullPath( path, nameFile, int_r ) );
	if (int_r != EOK) {
		return ENOMEM;
	}
	
	do {
		fatReturn = f_chmod( fullPath.get(), 0, AM_RDO|AM_ARC|AM_SYS|AM_HID );		/// Снимаем блокировки.
		if ( fatReturn != FR_OK ) break;
		fatReturn = f_unlink( fullPath.get() );
	} while( false );

	switch( ( uint32_t )fatReturn ) {
	case ( uint32_t )FR_OK:			return 0;
	case ( uint32_t )FR_NO_FILE:	return 1;
	default:
		return -1;
	}
}

int Fat::removeDir ( std::shared_ptr< char > path, FRESULT& fatReturn ) {
	FRESULT		r;
	do {
		r = f_chmod( path.get(), 0, AM_RDO|AM_ARC|AM_SYS|AM_HID );		/// Снимаем блокировки.
		if ( r != FR_OK ) break;
		r = f_unlink( path.get() );
	} while( false );

	switch( ( uint32_t )r ) {
	case ( uint32_t )FR_OK:			return 0;
	case ( uint32_t )FR_NO_FILE:	return 1;
	default:
		fatReturn = r;
		return -1;
	}
}

int Fat::removeDirRecurisve ( std::shared_ptr< char > fullPath, FRESULT& fatReturn ) {
	( void )fullPath;
	( void )fatReturn;
	static FILINFO			f;
	static int				r = 0;
	int mem_r;
	
	std::shared_ptr< DIR > d( Fat::openDir( fullPath, mem_r) );

	/// Что-то не так с драйверами или директории не существует.
	if ( d == nullptr )
		return -1;

	/// Рекурсивно обходим все папки.
	while( 1 ) {
		fatReturn = f_readdir( d.get(), &f );

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
			uint32_t i = strlen( fullPath.get() );
			sprintf( &fullPath.get()[ i ], "/%s", f.fname );

			r	=	Fat::removeDirRecurisve( fullPath, fatReturn );

			if ( fatReturn != FRESULT::FR_OK ) {								/// Аварийная ситуация.
				r = -1;
				break;
			}

			fullPath.get()[ i ] = 0;
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
}

}

