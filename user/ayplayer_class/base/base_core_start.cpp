#include "ayplayer_os_object.h"
#include "base.h"

namespace AyPlayer {

/*
int Base::fsmStepFuncCheckingChangeFatVolume ( void ) {
	//return this->checkingSystemFileInRootDir( "0:" );
	return 0;
}

int Base::fsmStepFuncCleanFlagChangeFatVolume ( void ) {
	//this->removeSystemFileInRootDir( "0:");
	return 0;
}

int Base::fsmStepFuncInitMainWindow ( void ) {
	/// Вместо считывания с карты памяти (системной).
	/// Заполняем стандартные параметры.
	this->lType			=	FILE_LIST_TYPE::NAME_SORT;
	this->playState		=	AYPLAYER_STATUS::STOP;
	this->currentFile	=	0;
	this->wNow			=	AYPLAYER_WINDOW_NOW::MAIN;

	/// Таблица громкости по умолчанию.
	memcpy( this->volumeTable, volumeTableDafault, sizeof( volumeTableDafault ) );
	this->currentVolumeIndex	=	4;

	/// Забираем начальные значения потенциометров.
	memcpy( &this->eq, &ayplayerEqualizerDafault, sizeof( ayplayerEqualizer ) );

	/// Переносим каталог воспроизведения в центр.
	FRESULT	fr;
	fr	=	f_chdir("0:");
	(void)fr;

	int		r;

	/// забираем колличество файлов в текущей директории.
	r = this->getFileCountInCurDir( this->lType, this->countFileInCurrentDir );

	/// Забираем данные о треке, на котором остановили воспроизведение.
	r = this->getFileInfoFromListCurDir( this->lType, this->currentFile );

	this->initPlayWindow();													/// Открываем главное окно.
	(void)r;


	this->guiUpdate();
	return 0;
}*/

}
