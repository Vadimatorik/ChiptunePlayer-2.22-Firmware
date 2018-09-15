#pragma once

#include "ayplayer_struct_cfg.h"
#include <memory>
#include <string>

#include "gui.h"
#include "nvic.h"
#include "rcc.h"
#include "sd_control.h"

#define	fatFsCheckAndReturn( fr )				\
			if ( fr != FRESULT::FR_OK ) {		\
				return -1;						\
			}

#if !defined( EOK )
#  define EOK 0         /* no error */
#endif

namespace AyPlayer {

enum class AYPLAYER_WINDOW_NOW {
	INIT				=	0,
	MAIN				=	1,
	EQUALIZER			=	2
};

#define HANDLER_FSM_STEP(NAME_STEP)				static int NAME_STEP ( const fsmStep< AyPlayer >* previousStep, AyPlayer* obj )
#define HANDLER_FSM_INPUT_DATA					__attribute__((unused)) const fsmStep< AyPlayer >* previousStep, AyPlayer* obj

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


/// Если на флешке нет актуальной таблицы
/// "уровень громкости == значение потенциометра" - тогда
/// используем стандартное.

const uint8_t volumeTableDafault[16] = {
	0x00, 0x10, 0x20, 0x30,
	0x40, 0x50, 0x60, 0x70,
	0x80, 0x90, 0xA0, 0xB0,
	0xC0, 0xD0, 0xE0, 0xFF
};


/// Значения эквалайзера (цифровых потенциометров).
/// 0..26 диапазон каждого поля.

struct __attribute__((packed)) ayplayerEqualizer {
	uint8_t			A1;
	uint8_t			B1;
	uint8_t			C1;
	uint8_t			A2;
	uint8_t			B2;
	uint8_t			C2;
};

const ayplayerEqualizer ayplayerEqualizerDafault = {
	.A1			=	230,
	.B1			=	200,
	.C1			=	230,
	.A2			=	230,
	.B2			=	200,
	.C2			=	230,
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
	/*
	 *	\brief		Метод производит инициализацию всей аппаратной
	 *				периферии микроконтроллера.
	 *				В случае, если что-то не удастся инициализировать
	 *				таким образом, чтобы дальнейшее приложение могла
	 *				функционировать хотя бы в ограниченном режиме -
	 *				производится перезагрузка процессора.
	 */
	void			initHardwareMc				(	void	);

	void			initTasks					(	void	);

	void			waitToInsertCorrectSdCard	(	void	);

private:
	uint32_t		getStatePlay				( void );
	uint32_t		getPercentBattery			( void );

private:
	static	void	mainTask						( void* obj );
	static void scrollNameInMainWindow ( TimerHandle_t timer );
	int initFileLists ( void );
	void			powerOff							(	CAUSE_SHUTDOWN	cause	);

	void			guiUpdate							( void );

	 /// Рисует сообщение об ошибке microsd.

	void			errorMicroSdDraw					( const FRESULT r );


	/// Проходится по всем каталогам и составляет список поддерживаемых файлов.

	FRESULT			indexingSupportedFiles				( char* path );


	void			initWindowIndexingSupportedFiles	( char* stateIndexing = nullptr );
	void			removeWindowIndexingSupportedFiles	( void );


	void			initGuiStatusBar					( void );
	void			slItemShiftDown							( uint32_t cout, char* newSt );

	int				scanDir								( char* path );

private:
	static void buttonClickHandlerTask ( void* obj );
	static void playTickHandlerTask ( void* obj );
	static void playTask ( void* obj );


private:
	int fsmStepFuncIndexingSupportedFiles ( void );
	int fsmStepFuncSortingFileList ( void );
	int fsmStepFuncCheckingChangeFatVolume ( void );
	int fsmStepFuncCleanFlagChangeFatVolume ( void );
	int fsmStepFuncInitMainWindow ( void );

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

	AyPlayerModuleGui g;



	AYPLAYER_STATUS										playState;
	FILE_LIST_TYPE										lType;
	AYPLAYER_WINDOW_NOW									wNow;
	uint32_t											currentFile;
	uint32_t											countFileInCurrentDir;

	/// Громкость.
	uint8_t												currentVolumeIndex;		/// Текущая громкость (значение потенциометра в таблице).
	uint8_t												volumeTable[16];		/// Соотношение "уровень громкости == значению потенциометра".

	ayplayerEqualizer									eq;

	AyPlayerFatFs										fat;

	/*!
	 *	\brief		Через данный объект производится взаимодействие
	 *				с аппаратным модулем NVIC.
	 */
	Nvic												nvic;

	AyPlayer::Rcc*										rcc;
	AyPlayer::Gui*										gui;
	AyPlayer::SdControl*								sd;
};

};
