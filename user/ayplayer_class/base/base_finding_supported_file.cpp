#include "base.h"
#include "fat/fat.h"

namespace AyPlayer {



int	Base::createFileListInSdCard ( std::shared_ptr< char >	path,
									WindowIndexingSupportedFiles*	w	) {
	int	r;
	std::shared_ptr< FILINFO >		fi( new FILINFO );
	std::shared_ptr< char >			findMsk( new char[ 10 ], std::default_delete< char[] >() );
	strcpy( findMsk.get(), "*.psg" );

	std::shared_ptr< DIR >		d( this->fat.openDirAndFindFirstFile( path, fi, findMsk, r ) );
	errnoCheckAndReturn( r );

	if ( d.get() == nullptr )
		return EOK;

	std::shared_ptr< FIL >		fileList( nullptr );

	while ( true ) {
		/// Просмотрены все файлы.
		if ( fi.get() == nullptr )
			break;

		/// Собираем строчку с полным путем.
		std::shared_ptr< char > fullPath ( this->fat.getFullPath( path, fi->fname, r ) );


		/// Лог: найден файл под маску.
		this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "File found:", fullPath.get() );				/// Лог.

		/// Экран: начат анализ файла.
		static const char state[] = "File analysis...";
		w->setActualState( state );
		this->gui->update();

		/// Проверяем правильность файла.
		uint32_t fileLen = 23;			/// 23 - это для теста.

		int rPsgGet = 0;
		rPsgGet = this->cfg->ay->psgFileGetLong( fullPath, fileLen );

		/// Если файл прошел проверку.
		if ( rPsgGet == EOK ) {
			/// Если в этой директории еще не создавали файл со списком прошедших проверку файлов.
			if ( fileList.get() == nullptr ) {
				std::shared_ptr< char > fullPathFileList ( this->fat.getFullPath( path, LIST_NO_SORT_FAT_NAME, r ) );
				errnoCheckAndReturn( r );

				fileList =  this->fat.openFileListWithRewrite( fullPathFileList, r );
				errnoCheckAndReturn( r );

				/// Если не удалось - чистим память и выходим.
				if ( r == EOK ) {
					this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "<<" LIST_NO_SORT_FAT_NAME ">> created in dir:", path.get() );
				} else {
					this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_ERROR, "<<" LIST_NO_SORT_FAT_NAME ">> not created in dir:", path.get() );
					return r;
				}
			}

			/// Лог: данный об проанализированном файле.
			this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "Analysis was carried out successfully:", fullPath.get() );
			char lenString[ 31 ];
			snprintf( lenString, 30, "%lu", fileLen );
			this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "File len tick:", lenString );

			std::shared_ptr< ItemFileInFat > item( this->fat.createtructureItemFileListFilling( fi->fname, fileLen, AyPlayFileFormat::psg, r ) );
			errnoCheckAndReturn( r );

			r	=	this->fat.writeFileListItem( fileList, item );

			if ( r == 0 ) {
				this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "Write item file in <<" LIST_NO_SORT_FAT_NAME ">> was been carried out successfully. Name item file: ", fi->fname );
			} else {
				this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_ISSUE, "Write item file in <<" LIST_NO_SORT_FAT_NAME ">> was not carried out successfully. Name item file: ", fi->fname );
				return EIO;
			}

			/// В списке новый файл, сдвигаем.
			w->addItem( fi->fname );
			this->gui->update();
		} else {
			this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_ISSUE, "Analysis was not carried out successfully:", fullPath.get() );
		}

		/// Ищем следующий элемент.
		fi = this->fat.findNextFileInDir( d, r );
		errnoCheckAndReturn( r );
	}

	if ( fileList.get() != nullptr ) {
		if ( this->fat.closeFile( fileList ) == EOK ) {
			this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_OK, "File <<" LIST_NO_SORT_FAT_NAME ">> was close successfully. Dir:" , path.get() );
		} else {
			this->printMessageAndArg( RTL_TYPE_M::RUN_MESSAGE_ISSUE, "File <<" LIST_NO_SORT_FAT_NAME ">> was not close successfully. Dir:" , path.get() );
		}
	}

	return r;
}

int Base::createFileListsInSdCard (	std::shared_ptr< char >			path,
									WindowIndexingSupportedFiles*	w			) {
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

			r	=	this->createFileListsInSdCard( path, w );
			errnoCheckAndReturn( r );

			/// Обрезаем строку, чтобы выйти из вложенного каталога.
			path.get()[ len ] = 0;

		} else {
			if ( scanDir == true ) continue;		/// Сканируем директорию лишь единожды.
			scanDir = true;
			if ( this->createFileListInSdCard( path, w ) != 0 ) {
				break;
			}
		}
	}

	return this->fat.closeDir( d );
}

}


