#pragma once

#include <memory>
#include <string>
#include <vector>
#include "gui.h"
#include "nvic.h"
#include "rcc.h"
#include "sd_control.h"
#include "user_os.h"
#include <errno.h>

#include "ayplayer_struct_cfg.h"
#include "user_os.h"
#include "ayplayer_button.h"


namespace AyPlayer {

enum class AYPLAYER_WINDOW_NOW {
	INIT				=	0,
	MAIN				=	1,
	EQUALIZER			=	2
};

#define	TB_MAIN_TASK_SIZE						800

#define	TB_BUTTON_CLICK_HANDLER_TASK_SIZE		400
#define	TB_PLAY_TASK_SIZE						800
#define	TB_PLAY_TICK_TASK_SIZE					200

#define	MAIN_TASK_PRIO							3
#define	BUTTON_CLICK_HANDLER_TASK_PRIO			1
#define	PLAY_TASK_PRIO							1
#define	PLAY_TICK_TASK_PRIO						1

#define SCROLL_STRING_NAME_LOW					1000
#define SCROLL_STRING_NAME_FAST					200

#define LIST_NO_SORT_FAT_NAME					".fileList.list"
#define LIST_SORT_NAME_FAT_NAME					".fileListNameSort.list"
#define LIST_SORT_LEN_FAT_NAME					".fileListLenSort.list"

/*!
 *	\brief		Максимальная длинна строки-пути в fatfs.
 *				Важно: при выделении памяти следует выделять на 1 байт больше.
 */
#define MAX_PATH_FATFS_STRING_LEN				1024


enum class AYPLAYER_STATUS {
	STOP				=	0,
	PLAY				=	1,
	PAUSE				=	2
};

enum class FILE_LIST_TYPE {
	NO_SORT			=	0,
	NAME_SORT		=	1,
	LEN_SORT		=	2,
};

/// Причина отключения плеера.

enum class CAUSE_SHUTDOWN {
	USER_OFF				=	0,
	UNDERVOLTAGE			=	1,
	TIMEOUT					=	2
};

#include "ayplayer_struct_cfg.h"

class Base {
public:
	Base( const BaseCfg* const cfg );

public:
	void			start						(	void	);

private:
	/*!
	 *	\brief		Метод проверяет результат выполнения
	 *				предыдущей операции и в случае, если
	 *				он не McHardwareInterfaces::BaseResult::ok
	 *				производит аварийную перезагрузку устройства.
	 *
	 *	\param[in]	resultValue	-	возвращенный предыдущим по ходу выполнения
	 *								программы методом результат выполнения.
	 *
	 */
	void	checkAndExit				(	McHardwareInterfaces::BaseResult	resultValue	);

private:
	/*!
	 *	\brief		Метод производит запись в log сообщания (строки).
	 *
	 *	\param[in]	type		-	тип записываемого в log сообщения.
	 *	\param[in]	message		-	строка с текстом записываемого сообщения.
	 *								Примечание:	после вызова метода
	 *											может быть удалена.
	 *
	 *	\return		EOK			-	Успешность операции.
	 *								В противном случае код из errno.h.
	 *
	 */
	int		printMessage						(	RTL_TYPE_M		type,
													const char*		const message	);

	/*!
	 *	\brief		Метод производит запись в log строки
	 *				с следующим за ним параметром.
	 *
	 *	\param[in]	type		-	тип записываемого в log сообщения.
	 *	\param[in]	message		-	строка с текстом записываемого сообщения.
	 *								Примечание:	после вызова метода
	 *											может быть удалена.
	 *	\param[in]	arg			-	аргумент (строка) следующий за сообщением.
	 *								Примечание:	после вызова метода
	 *											может быть удалена.
	 *
	 *	\return		EOK			-	Успешность операции.
	 *								В противном случае код из errno.h.
	 */
	int		printMessageAndArg					(	RTL_TYPE_M		type,
													const char*		const message,
													const char*		const arg		);

private:
	/*!
	 *	\brief		Метод проходится по всем вложенным каталогам
	 *				начиная с заданного пути и для каждого каталога
	 *				вызывает метод createFileListInSdCard.
	 *
	 *	\param[in]	path		-	Директория, с которой начинается анализ всех
	 *								файлов рекурсивно вниз по дереву.
	 *
	 *	\return		EOK			-	Успешность операции.
	 *								В противном случае код из errno.h.
	 */
	int		createFileListsInSdCard				(	std::shared_ptr< char >		path,
													WindowIndexingSupportedFiles*	w	);

	/*!
	 *	\brief		Метод производит анализ заданной директории и в случае,
	 *				если в ней есть подходящие по маске файлы - вызывает
	 *				методы их проверки на валидность и в случае успешности
	 *				последней создает файл списка и добавляет имя последнего
	 *				прошедшего файла в список.
	 *
	 *	\param[in]	path		-	Директория, в которой производится анализ файлов.
	 *
	 *	\return		EOK			-	Успешность операции.
	 *								В противном случае код из errno.h.
	 */
	int		createFileListInSdCard				(	std::shared_ptr< char >		path,
													WindowIndexingSupportedFiles*	w	);

	int enterInDirWitchFindingFileListAndSort (	std::shared_ptr< char >		path,
														std::shared_ptr< FILINFO >		f,
														WindowIndexingSupportedFiles*	w);
private:
	/*
	 *	\brief		Метод производит инициализацию всей аппаратной
	 *				периферии микроконтроллера.
	 *				В случае, если что-то не удастся инициализировать
	 *				таким образом, чтобы дальнейшее приложение могла
	 *				функционировать хотя бы в ограниченном режиме -
	 *				производится перезагрузка процессора.
	 *
	 *
	 */
	void			initHardwareMc				(	void	);

	void			initTasks					(	void	);

	void			waitToInsertCorrectSdCard	(	void	);

	std::shared_ptr< ItemFileInFat > createtructureItemFileListFilling (	const char*			const nameTrack,
																			uint32_t			lenTickTrack,
																			AyPlayFileFormat	format,
																			int&				returnResult	);

	int sortForNameFileList				(	std::shared_ptr< char >		path,
													std::shared_ptr< FIL >		fNoSort,
													std::shared_ptr< FIL >		fSortFile,
													std::vector< uint16_t >		fl			);

	int sortForLenFileList				(	std::shared_ptr< char >		path,
													std::shared_ptr< FIL >		fNoSort,
													std::shared_ptr< FIL >		fSortFile,
													std::vector< uint16_t >		fl			) ;

	int writeSortFile (	std::shared_ptr< FIL >		fNoSort,
								std::shared_ptr< FIL >		fSort,
								std::vector< uint16_t >		fl	);

	/// Устанавливает параметры из флешки.
	int			setSysCfgParams				(	void	);
	int setSyscfgDefaultGuiParams ( void );

	int	findingFileListAndSort (	std::shared_ptr< char >		path,
			WindowIndexingSupportedFiles*	w);
	int	sortFileList ( std::shared_ptr< char >		path );
	int	sortFileListCreateFiles (	std::shared_ptr< char >		path,
										std::shared_ptr< FIL >&		fNoSort,
										std::shared_ptr< FIL >&		fNameSort,
										std::shared_ptr< FIL >&		fLenSort	);
	std::vector< uint16_t > initPointArrayToSort ( uint32_t count );

	int initMainWindow ( void );

private:
	uint32_t		getStatePlay				( void );
	uint32_t		getPercentBattery			( void );

private:

private:
	static	void	mainTask						( void* obj );
	static void scrollNameInMainWindow ( TimerHandle_t timer );
	int initFileLists ( void );
	void			powerOff							(	CAUSE_SHUTDOWN	cause	);

	void			guiUpdate							( void );

	 /// Рисует сообщение об ошибке microsd.

	void			errorMicroSdDraw					( const FRESULT r );


	/// Проходится по всем каталогам и составляет список поддерживаемых файлов.

	void			removeWindowIndexingSupportedFiles	( void );


	void			initGuiStatusBar					( void );
	void			slItemShiftDown							( uint32_t cout, char* newSt );



private:
	static	void	playTickHandlerTask					(	void*		obj		);
	static	void	buttonClickHandlerTask				( 	void*		obj 	);
	static	void	playTask							(	void*		obj		);

private:
	const BaseCfg* const cfg;

	USER_OS_STATIC_STACK_TYPE							tbMainTask[ TB_MAIN_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE						tsMainTask;

	USER_OS_STATIC_STACK_TYPE							tbButtonClickHandlerTask[ TB_BUTTON_CLICK_HANDLER_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE						tsButtonClickHandlerTask;
	USER_OS_STATIC_STACK_TYPE							tbPlayTask[ TB_PLAY_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE						tsPlayTask;
	USER_OS_STATIC_STACK_TYPE							tbPlayTickTask[ TB_PLAY_TICK_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE						tsPlayTickTask;

	USER_OS_TIMER										timNameScroll;
	USER_OS_TIMER_STATIC_STRUCT							timStNameScroll;

	AYPLAYER_STATUS										playState;
	FILE_LIST_TYPE										lType;
	AYPLAYER_WINDOW_NOW									wNow;
	uint32_t											currentFile;
	uint32_t											countFileInCurrentDir;
	std::shared_ptr< ItemFileInFat >					playItem;

	AyPlayer::Fat										fat;
	AyPlayer::Nvic										nvic;
	AyPlayer::Rcc*										rcc;
	AyPlayer::Gui*										gui;
	AyPlayer::SdControl*								sd;
};

};
