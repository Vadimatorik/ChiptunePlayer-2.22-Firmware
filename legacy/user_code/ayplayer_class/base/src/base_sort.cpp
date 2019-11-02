#include <algorithm>
#include <vector>
#include "base.h"
#include "fat.h"

namespace AyPlayer {

int Base::writeSortFile (	std::shared_ptr< FIL >		fNoSort,
							std::shared_ptr< FIL >		fSort,
							std::vector< uint16_t >		fl	) {
	int funResult;

	/// Пишем все структуры.
	for ( uint16_t inItemPoint : fl ) {
		std::shared_ptr< ItemFileInFat > inputItem( this->fat.readItemFileList( fNoSort, inItemPoint, funResult ) );
		if ( funResult != EOK )	break;	/// Что-то с картой.
		funResult = this->fat.writeFileListItem( fSort, inputItem  );
		if ( funResult != EOK )	break;	/// Что-то с картой.
	}

	if ( funResult != EOK )
		return funResult;

	funResult = this->fat.closeFile( fSort );
	if ( funResult != EOK )
		return funResult;

	return EOK;
}



int Base::sortForNameFileList				(	std::shared_ptr< char >		path,
												std::shared_ptr< FIL >		fNoSort,
												std::shared_ptr< FIL >		fSortFile,
												std::vector< uint16_t >		fl			) {
	int	sortResult	=	EOK;

	/// Начинаем быструю сортировку по имени.
	std::sort( fl.begin(), fl.end(), [ this, path, &sortResult, fNoSort ]( uint16_t a, uint16_t b ) {
		/// Если в процессе сортировки упадет флешка -
		/// то выдаем одно значение для быстроты сортировки и выходим.
		/// Так массив быстро отсортируется и выйдя мы поймем, что упали.
		if ( sortResult != EOK ) {
			return true;
		}

		/// Получаем имена треков.
		std::shared_ptr< char > aName( this->fat.getNameTrackFromFile( fNoSort, a, sortResult ) );
		if ( sortResult ) return true;	/// Вранье, чтобы закончить анализ.

		std::shared_ptr< char > bName( this->fat.getNameTrackFromFile( fNoSort, b, sortResult ) );
		if ( sortResult ) return true;	/// Вранье, чтобы закончить анализ.

		/// Сравниваем строки.
		int resStrcmp;
		resStrcmp	=	strcmp( aName.get(), bName.get() );

		return resStrcmp < 0;
	} );

	if ( sortResult == EOK ) {
		sortResult = this->writeSortFile( fNoSort, fSortFile, fl );
	}

	return sortResult;
}


int Base::sortForLenFileList				(	std::shared_ptr< char >		path,
												std::shared_ptr< FIL >		fNoSort,
												std::shared_ptr< FIL >		fSortFile,
												std::vector< uint16_t >		fl			) {
	int	sortResult	=	EOK;

	/// Начинаем быструю сортировку по имени.
	std::sort( fl.begin(), fl.end(), [ this, path, &sortResult, fNoSort ]( int a, int b ) {
		/// Если в процессе сортировки упадет флешка -
		/// то выдаем одно значение для быстроты сортировки и выходим.
		/// Так массив быстро отсортируется и выйдя мы поймем, что упали.
		if ( sortResult != EOK ) {
			return true;
		}

		/// Получаем имена треков.
		uint32_t aLen = this->fat.getSizeTrackFromFile( fNoSort, a, sortResult );
		if ( sortResult ) return true;	/// Вранье, чтобы закончить анализ.

		uint32_t bLen = this->fat.getSizeTrackFromFile( fNoSort, b, sortResult );
		if ( sortResult ) return true;	/// Вранье, чтобы закончить анализ.

		return aLen < bLen;
	} );

	if ( sortResult == EOK ) {
		sortResult = this->writeSortFile( fNoSort, fSortFile, fl );
	}

	return sortResult;
}

int	Base::sortFileListCreateFiles (	std::shared_ptr< char >		path,
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

std::vector< uint16_t > Base::initPointArrayToSort ( uint32_t count ) {
	std::vector< uint16_t > v;
	/// Изначально все строкутры в своем порядке 0..countFileInDir-1.
	for ( uint16_t i = 0; i < count; i++ )
		v.push_back( i );
	return v;
}


int	Base::sortFileList							(	std::shared_ptr< char >			path	) {
	int								r;

	std::shared_ptr< FIL >			fNoSort		( nullptr );
	std::shared_ptr< FIL >			fNameSort	( nullptr );
	std::shared_ptr< FIL >			fLenSort	( nullptr );

	r = sortFileListCreateFiles( path, fNoSort, fNameSort, fLenSort );
	errnoCheckAndReturn( r );

	this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "Start sorting in dir: ", path.get() );

	/// Получаем колличество файлов в директории.
	const uint32_t	countFileInDir	=	this->fat.getNumberTrackInList( fNoSort );

	/// Для каждой структуры выделим свой номер, так как они расположены в исходном файле.
	std::vector< uint16_t >	fl;

	/// Сортировка по имени.
	fl = initPointArrayToSort( countFileInDir );
	r = sortForNameFileList( path, fNoSort, fNameSort, fl );
	errnoCheckAndReturn( r );

	/// Сортировка по длине.
	fl = initPointArrayToSort( countFileInDir );
	r = sortForLenFileList( path, fNoSort, fLenSort, fl  );
	errnoCheckAndReturn( r );

	return r;
}


int Base::enterInDirWitchFindingFileListAndSort	(	std::shared_ptr< char >			path,
													std::shared_ptr< FILINFO >		f,
													WindowIndexingSupportedFiles*	w		) {
	int			r;
	uint32_t	len	=	strlen( path.get() );

	snprintf( path.get(), MAX_PATH_FATFS_STRING_LEN, "%s/%s", path.get(), f->fname );

	r	=	this->findingFileListAndSort( path, w );

	/// Обрезаем строку, чтобы выйти из вложенного каталога.
	path.get()[ len ] = 0;

	return r;
}

int Base::findingFileListAndSort				(	std::shared_ptr< char >			path,
													WindowIndexingSupportedFiles*	w		) {
		int					r;

		/// Флаг выставляется, когда мы обнаружили в
		/// директории хоть один файл и просканировали ее на все поддерживаемые форматы
		/// по шаблону.
		/// Чтобы не нраваться на многократное повторное сканирование.
		bool				scanDir	=	false;

		/// Открываем директорию.
		std::shared_ptr< DIR >			d( this->fat.openDir( path, r ) );
		errnoCheckAndReturn( r );

		this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "Open dir: ", path.get() );

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
				r = enterInDirWitchFindingFileListAndSort( path, f, w );
				errnoCheckAndReturn( r );
			} else {
				if ( scanDir == true ) continue;				/// Сканируем директорию лишь единожды.
				scanDir = true;
				if ( this->sortFileList( path ) != 0 ) {
					break;
				}
			}
		}

		return this->fat.closeDir( d );
}

}

