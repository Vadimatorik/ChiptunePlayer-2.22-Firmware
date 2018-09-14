#include "ayplayer_fat.h"
#include <string.h>
#include <stdio.h>
#include "FreeRTOSConfig.h"

namespace AyPlayer {

char* Fat::getFullPath( const char* const path, const char* const fileName ) {
	/// Получаем длину каждой строки без учета нуля терминатора.
	const uint32_t pathLen		=	strlen( path );
	const uint32_t nameFileLen	=	strlen( fileName );

	/// Получаем длину полной строки с учетом символа "/" и нуля терминатора.
	const uint32_t allLen = pathLen + nameFileLen + 2;

	/// Выделяем память под строку.
	char* allPathToFile	=	( char* )pvPortMalloc( allLen );
	assertParam( allPathToFile );

	/// Собираем единую строку.
	sprintf( allPathToFile, "%s/%s", path, fileName );

	/// Возвращаем указатель на полную строку.
	return allPathToFile;
}

DIR* Fat::openDir( char* path ) {
	FRESULT			r;
	DIR*			d;

	d	=	( DIR* )pvPortMalloc( sizeof( DIR ) );
	assertParam( d );

	/// Открываем директорию.
	r = f_opendir( d, path );

	/// Если открыть директорию не удалось - высвобождаем память.
	if ( r != FRESULT::FR_OK ) {
		vPortFree( d );
		d	=	nullptr;
	}

	return d;
}

int Fat::closeDir( DIR* d ) {
	if ( d == nullptr )		return 0;

	FRESULT			r;

	/// Закрываем директорию.
	r = f_closedir( d );
	vPortFree( d );

	/// Возвращаем успешность операции.
	return ( r == FRESULT::FR_OK ) ? 0 : -1;
}

FIL* Fat::openFileListWithRewrite ( const char* const path, const char* const name ) {
	FRESULT				r;
	FIL*				f;

	/// Выделяем память под объект файла FatFS.
	f	=	( FIL* )pvPortMalloc( sizeof( FIL ) );
	assertParam( f );


	char* fullPath	=	Fat::getFullPath( path, name );
	/// Пытаемся открыть файл с перезаписью, если таковой ранее существовал.
    r = f_open( f, fullPath, FA_CREATE_ALWAYS | FA_READ | FA_WRITE );
    vPortFree( fullPath );

    if ( r != FR_OK ) {
    	vPortFree( f );
    	f = nullptr;
    }

    return f;
}

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

int Fat::closeFile ( FIL* f ) {
	if ( f == nullptr )		return 0;

	FRESULT				r;

	/// Пытаемся открыть файл с перезаписью, если таковой ранее существовал.
    r = f_close( f );
    vPortFree( f );

    return ( r == FRESULT::FR_OK ) ? 0 : -1;
}

int	Fat::writeItemFileListAndRemoveItem	( FIL* f, ItemFileInFat* item ) {
	FRESULT				r;

	/// Пробуем записать.
	UINT				l;
	r = f_write( f, item, sizeof( ItemFileInFat ), &l );

	/// После записи нам уже не нужна структура.
	vPortFree( item );

    /// Если записалось целиком все успешно, то возвратим 0.
    return ( ( r == FR_OK ) && ( l == sizeof( ItemFileInFat ) ) ) ? 0 : -1;
}

int Fat::startFindingFileInDir ( DIR** dir, FILINFO** fInfo, const char* const pathDir, const char* const maskFind ) {
	FRESULT				r;

	/// Выделяем память под объект директории, в которой будем искать файл,
	DIR*		d						=	( DIR* )pvPortMalloc( sizeof( DIR ) );
	assertParam( d );

	/// Выделяем память под объект директории, в которой будем искать файл,
	FILINFO*	fi						=	( FILINFO* )pvPortMalloc( sizeof( FILINFO ) );
	assertParam( fi );

	/// Открываем директорию и ищем в ней файл по маске.
	r = f_findfirst( d, fi, pathDir, maskFind );

	/// Файл найден.
	if ( r == FR_OK && fi->fname[0] ) {
		*dir	=	d;
		*fInfo	=	fi;		/// Отдаем указатель на структуру информации о файле FatFS.

		return 0;
	}

	/// Пошли проблемы, объекты директории и информации о файле теперь не нужны.
	vPortFree( d );
	vPortFree( fi );

	/// Проблемы на уровне флешки.
	if ( r != FR_OK ) {
		return -1;
	}

	/// Просто нет такого файла.
	return 1;
}

int Fat::findingFileInDir ( DIR* d, FILINFO* fInfo ) {
	FRESULT				r;

	/// Ищем следующий файл по маске.
	r = f_findnext( d, fInfo );

	/// Файл найден.
	if ( r == FR_OK && fInfo->fname[0] ) {
		return 0;
	}

	/// Пошли проблемы, объекты директории и информации о файле теперь не нужны.
	vPortFree( d );
	vPortFree( fInfo );

	/// Проблемы на уровне флешки.
	if ( r != FR_OK ) {
		return -1;
	}

	/// Просто нет такого файла.
	return 1;
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
}

}

