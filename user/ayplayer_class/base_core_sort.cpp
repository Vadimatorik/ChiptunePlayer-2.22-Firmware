#include <algorithm>
#include <vector>
#include "base.h"
#include "fat.h"

namespace AyPlayer {


int	Base::sortFileListCreateFile (	std::shared_ptr< char >		path,
									std::shared_ptr< FIL >&		fNoSort,
									std::shared_ptr< FIL >&		fNameSort,
									std::shared_ptr< FIL >&		fLenSort	) {
	int r = EOK;

	do {
		/// Открываем файл со списком.
		static char listNoSort[] = LIST_NO_SORT_FAT_NAME;
		fNoSort	=	this->fat.openFile( path, listNoSort, r );
		errnoCheckAndReturn( r );

		if ( fNoSort.get() != nullptr ) {
			this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "File <<" LIST_NO_SORT_FAT_NAME ">> opened successfully in dir:", path.get() );
		} else {
			this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_ERROR, "File <<" LIST_NO_SORT_FAT_NAME ">> not been open in dir:", path.get() );
			r = EINVAL;
			break;
		}

		/// Отсортированные по именам.
		static char listNameSort[] = LIST_SORT_NAME_FAT_NAME;
		fNameSort	=	this->fat.openFileListWithRewrite( path, listNameSort, r );
		errnoCheckAndReturn( r );
		this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "File <<" LIST_SORT_NAME_FAT_NAME ">> opened successfully in dir:", path.get() );

		/// По длине.
		static char listLenSort[] = LIST_SORT_LEN_FAT_NAME;
		fLenSort	=	this->fat.openFileListWithRewrite( path, listLenSort, r );
		errnoCheckAndReturn( r );
		this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "File <<" LIST_SORT_NAME_FAT_NAME ">> opened successfully in dir:", path.get() );

	}  while( false );

	if ( r != EOK )	{
		this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_ERROR, "File <<" LIST_NO_SORT_FAT_NAME ">>, <<" LIST_SORT_NAME_FAT_NAME ">> and <<" LIST_SORT_LEN_FAT_NAME ">> are closed in an emergency! Dir:", path.get() );
		return r;
	}

	return EOK;
}

int	Base::sortFileListCloseFile ( const char* const path, DIR* d, FILINFO* fi, FIL* fNoSort, FIL* fNameSort, FIL* fLenSort ) {
	int r = 0;
	if ( fi )										vPortFree( fi );
	r	=	( AyPlayerFat::closeDir( d )			!= 0 ) ? -1 : r;
	r	=	( AyPlayerFat::closeFile( fNoSort )		!= 0 ) ? -1 : r;
	r	=	( AyPlayerFat::closeFile( fNameSort )	!= 0 ) ? -1 : r;
	r	=	( AyPlayerFat::closeFile( fLenSort )	!= 0 ) ? -1 : r;

	if ( r == 0 ) {
		this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "File <<" LIST_NO_SORT_FAT_NAME ">>, <<" LIST_SORT_NAME_FAT_NAME ">> and <<" LIST_SORT_LEN_FAT_NAME ">> closed successfully! Dir:", path );
	} else {
		this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_ERROR, "File <<" LIST_NO_SORT_FAT_NAME ">>, <<" LIST_SORT_NAME_FAT_NAME ">> and <<" LIST_SORT_LEN_FAT_NAME ">> are closed in an emergency! Dir:", path );
	}

	return r;
}

int Base::writeSortFile ( FIL* output, FIL* input, uint16_t* sortArray, uint32_t count ) {
	int funResult;

	/// Пишем все структуры.
	for ( uint32_t i = 0; i < count; i++ ) {
		/// Выделяем память под считываемый из исходного файла элемент.
		ItemFileInFat*	item	=	( ItemFileInFat* )pvPortMalloc( sizeof( ItemFileInFat ) );
		assertParam( item );

		funResult = AyPlayerFat::readItemFileListAndRemoveItem( input, item, sortArray[ i ] );
		if ( funResult != 0 )	break;	/// Что-то с картой.
		funResult = AyPlayerFat::writeItemFileListAndRemoveItem( output, item );
		if ( funResult != 0 )	break;	/// Что-то с картой.
	}

	return funResult;
}

void Base::initPointArrayToSort ( uint16_t* array, uint32_t count ) {
	/// Изначально все строкутры в своем порядке 0..countFileInDir-1.
	for ( uint32_t i = 0; i < count; i++ )
		array[ i ] = i;
}


int Base::sortForNameFileList ( const char* const path, uint16_t* fl, uint32_t countFileInDir, FILINFO* fi, DIR* d, FIL* fNoSort, FIL* fNameSort ) {
	int	sortResult	=	0;
	/// Начинаем быструю сортировку по имени.
	std::sort( fl, &fl[ countFileInDir ], [ this, path, &sortResult, fi, d, fNoSort, fNameSort ]( int a, int b ) {

	 	 * Если в процессе сортировки упадет флешка - то выдаем одно значение для быстроты сортировки и выходим.
	 	 * Так массив быстро отсортируется и выйдя мы поймем, что упали.

		if ( sortResult < 0 ) {
			return true;
		}

		/// Получаем имена треков.
		char*	aStruct	=	AyPlayerFat::getNameTrackFromFile( fNoSort, a );

		if ( aStruct == nullptr ) {
			sortResult	=	-1;
			return true;
		}

		char*	bStruct	=	AyPlayerFat::getNameTrackFromFile( fNoSort, b );
		if ( bStruct == nullptr ) {
			sortResult	=	-1;
			return true;
		}

		/// Сравниваем строки.
		int resStrcmp;
		resStrcmp	=	strcmp( aStruct, bStruct );

		/// После сравнения удаляем сравниваемые элементы.
		vPortFree( aStruct );
		vPortFree( bStruct );

		/// Возвращаем результат сравнения.
		return resStrcmp < 0;
	} );

	if ( sortResult == 0 ) {
		sortResult = this->writeSortFile( fNameSort, fNoSort, fl, countFileInDir );
	}

	return sortResult;
}

int Base::sortForLenFileList ( const char* const path, uint16_t* fl, uint32_t countFileInDir, FILINFO* fi, DIR* d, FIL* fNoSort, FIL* fLenSort ) {
	int	sortResult	=	0;

	/// Начинаем быструю сортировку по имени.
	std::sort( fl, &fl[ countFileInDir ], [ this, path, &sortResult, fi, d, fNoSort, fLenSort ]( int a, int b ) {

	 	 * Если в процессе сортировки упадет флешка - то выдаем одно значение для быстроты сортировки и выходим.
	 	 * Так массив быстро отсортируется и выйдя мы поймем, что упали.

		if ( sortResult < 0 ) {
			return true;
		}

		/// Получаем имена треков.
		uint32_t	aLen	=	AyPlayerFat::getSizeTrackFromFile( fNoSort, a );

		if ( aLen == 0xFFFFFFFF ) {
			sortResult	=	-1;
			return true;
		}

		uint32_t	bLen	=	AyPlayerFat::getSizeTrackFromFile( fNoSort, b );
		if ( bLen == 0xFFFFFFFF ) {
			sortResult	=	-1;
			return true;
		}

		return aLen < bLen;
	} );

	if ( sortResult == 0 ) {
		sortResult = this->writeSortFile( fLenSort, fNoSort, fl, countFileInDir );
	}

	return sortResult;
}

int	Base::sortFileList ( std::shared_ptr< char >		path ) {
	int					r;

	std::shared_ptr< FILINFO >		fi( new FILINFO );

	std::shared_ptr< FIL >	fNoSort		( new FIL );
	std::shared_ptr< FIL >	fNameSort	( new FIL );
	std::shared_ptr< FIL >	fLenSort	( new FIL );

	/// Ищем файл в директории.
	std::shared_ptr< char >	msk( new char[ strlen( LIST_NO_SORT_FAT_NAME ) + 1 ], std::default_delete< char[] >() );
	strcpy( msk.get(), LIST_NO_SORT_FAT_NAME );
	std::shared_ptr< DIR >	d( this->fat.openDirAndFindFirstFile( path, fi, msk, r );
	errnoCheckAndReturn( r );

	if ( sortFileListCreateFile( path, fNoSort, fNameSort, fLenSort ) != EOK ) {
		return this->fat.closeDir( d );
	}

	std::shared_ptr< char > fullPath = this->fat.getFullPath( path, fi->fname, r );
	errnoCheckAndReturn( r );
	this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "Start sorting file:", fullPath.get() );

	/// Получаем колличество файлов в директории.
	const uint32_t	countFileInDir	=	this->fat.getFileSize( fNoSort );


	/// Для каждой структуры выделим свой номер, так как они расположены в исходном файле.
	uint16_t* fl	=	new uint16_t[ countFileInDir ];

	/// Сортировка по имени.
	this->initPointArrayToSort( fl, countFileInDir );
	r = sortForNameFileList( path, fl, countFileInDir, fi, d, fNoSort, fNameSort );
	if ( r != 0 ) {
		vPortFree( fl );
		this->sortFileListCloseFile( path, d, fi, fNoSort, fNameSort, fLenSort );
		return r;
	}

	/// Сортировка по длине.
	this->initPointArrayToSort( fl, countFileInDir );
	r = sortForLenFileList( path, fl, countFileInDir, fi, d, fNoSort, fLenSort );
	if ( r != 0 ) {
		vPortFree( fl );
		this->sortFileListCloseFile( path, d, fi, fNoSort, fNameSort, fLenSort );
		return r;
	}

	vPortFree( fl );

	/// Во время сортировки случилась ошибка.
	if ( r != 0 ) {
		this->sortFileListCloseFile( path, d, fi, fNoSort, fNameSort, fLenSort );
		return -1;
	}

	r = this->sortFileListCloseFile( path, d, fi, fNoSort, fNameSort, fLenSort );
	return r;
}

int Base::findingFileListAndSort ( std::shared_ptr< char >		path ) {
		int					r;

		/// Флаг выставляется, когда мы обнаружили в
		/// директории хоть один файл и просканировали ее на всё по шаблону.
		/// Чтобы не нраваться на многократное повторное сканирование.
		bool				scanDir	=	false;

		/// Открываем директорию.
		std::shared_ptr< DIR >			d( this->fat.openDir( path, r ) );
		errnoCheckAndReturn( r );

		this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "Open dir:", path.get() );

		/// Рекурсивно обходим все папки.
		while( 1 ) {
			static const char state[] = "Find supported files...";
			w->setActualState( state );
			this->gui->update();

			std::shared_ptr< FILINFO >		f( this->fat.readDir( d, r ) );
			errnoCheckAndReturn( r );

			/// Закончились элементы в текущей директории.
			if ( f.get() == nullptr ) {
				this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "Close dir:", path.get() );
				break;
			}

			/// Найдена новая директория.
			if ( this->fat.checkFileIsDir( f ) ) {
				uint32_t	len	=	strlen( path.get() );

				snprintf( path.get(), MAX_PATH_FATFS_STRING_LEN, "%s/%s", path.get(), f->fname );

				r	=	this->findingFileListAndSort( path );
				errnoCheckAndReturn( r );

				/// Обрезаем строку, чтобы выйти из вложенного каталога.
				path.get()[ len ] = 0;

			} else {
				if ( scanDir == true ) continue;		/// Сканируем директорию лишь единожды.
				scanDir = true;
				if ( this->sortFileList( path ) != 0 ) {
					break;
				}
			}
		}

		return this->fat.closeDir( d );
}

}

