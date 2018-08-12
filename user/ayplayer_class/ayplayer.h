#pragma once

#include "ayplayer_struct_cfg.h"

enum class AYPLAYER_WINDOW_NOW {
	INIT				=	0,
	MAIN				=	1,
	EQUALIZER			=	2
};


#define HANDLER_FSM_STEP(NAME_STEP)				static int NAME_STEP ( const fsmStep< AyPlayer >* previousStep, AyPlayer* obj )
#define HANDLER_FSM_INPUT_DATA					__attribute__((unused)) const fsmStep< AyPlayer >* previousStep, AyPlayer* obj

#define	TB_MAIN_TASK_SIZE						800
#define	TB_ILLUMINATION_CONTROL_TASK_SIZE		200
#define	TB_BUTTON_CLICK_HANDLER_TASK_SIZE		400
#define	TB_PLAY_TASK_SIZE						800
#define	TB_PLAY_TICK_TASK_SIZE					200

#define	MAIN_TASK_PRIO							3
#define	ILLUMINATION_CONTROL_TASK_PRIO			1
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

class AyPlayer {
public:
	AyPlayer( const AyPlayerCfg* const cfg ) :
		cfg( cfg ) {}

	void			start						( void );


	/// Возвращает текущий режим работы RCC.

	uint32_t		getRccMode					( void );

	uint32_t		getStatePlay				( void );
	uint32_t		getPercentBattery			( void );

private:
	static	void	mainTask						( void* obj );
	static void scrollNameInMainWindow ( TimerHandle_t timer );

	void			powerOff							(	CAUSE_SHUTDOWN	cause	);


/*
	 * Останавливает все аппаратные модули, зависящие от тактового сигнала,
	 * пытается установить заданную тактовую частоту на шинах, после чего
	 * конфигурирует заново все объекты, зависящие от частоты тактового сигнала
	 * ( вызывается метод reinitObjDependingRcc ).
	 *
	 * Замечание: в случае, если не удалось настроить заданную конфигурацию - микроконтроллер переходит
	 * в режим работы по умолчанию.
*/
	RCC_RESULT		setRccCfg							( uint32_t numberCfg );


	 // Отключает все объекты, зависящие
	 // от частоты тактового сигнала.

	void			offObjDependingRcc					( void );


	 // Метод переконфигурирует все объекты, зависящие
	 // от частоты тактового сигнала.

	void			reinitObjDependingRcc				( void );


	 // Запускает базовые интерфейсы:
	 // SPI, UART, ADC (канал аккумулятора).

	void			startBaseInterfaces					( void );


	 // Инициализирует RCC с максимально возможной скоростью.

	int			rccMaxFrequancyInit					( void );

/*
	 * Возвращает флаг наличия запрошенной microsd в слоте.
	 * \param[in]	sd	-	проверяемая карта.
	 * \return		{	true	-	карта обнаружена..
	 * 					false	-	в противном случае.}
	 * 					*/

	bool			checkSd								( void );


	 /// Ожидает, пока будут вставлены обе microsd карты.
	/// Выводит на экран и в лог соответвующие выводы.

	void			waitSdCardInsert					( void );


	 /// Перерисовывает экран и обновляет буфер в экране..

	void			guiUpdate							( void );


	/// Попытка инициализировать FatFS для выбранной карты.

	FRESULT			fatFsReinit							( void );


	 /// Рисует сообщение об ошибке microsd.

	void			errorMicroSdDraw					( const FRESULT r );


	 /// Ждем, пока отсоединят флешку.

	void			waitSdCardDisconnect				( void );


	/// Проходится по всем каталогам и составляет список поддерживаемых файлов.

	FRESULT			indexingSupportedFiles				( char* path );

	void			printMessageAndArg					( RTL_TYPE_M type, const char* const message, const char* const arg );

	void			initWindowIndexingSupportedFiles	( char* stateIndexing = nullptr );
	void			removeWindowIndexingSupportedFiles	( void );


	void			initGuiStatusBar					( void );
	void			slItemShiftDown							( uint32_t cout, char* newSt );

	int				scanDir								( char* path );

	/// 0 - успех. -1 провал
	int				writeItemFileList					( FIL* f, const uint32_t* const len, const AyPlayFileFormat format );

	void			slItemClean							( uint32_t cout );
	ItemFileInFat*	structureItemFileListFilling		( const char* const nameTrack, const uint32_t lenTickTrack, const AyPlayFileFormat format );
	int				sortFileList						( char* path );
	FRESULT			findingFileListAndSort				( char* path );
	int				sortFileListCreateFile				( const char* const path, FIL** fNoSort, FIL** fNameSort, FIL** fLenSort );
	int				sortFileListCloseFile				( const char* const path, DIR* d, FILINFO* fi, FIL* fNoSort, FIL* fNameSort, FIL* fLenSort );
	int				writeSortFile						( FIL* output, FIL* input, uint16_t* sortArray, uint32_t count );
	void			initWindowSortingFileList			( void );
	void			removeWindowSortingFileList			( void );
	void			initPointArrayToSort				( uint16_t* array, uint32_t count );
	int				sortForNameFileList					( const char* const path, uint16_t* fl, uint32_t countFileInDir, FILINFO* fi, DIR* d, FIL* fNoSort, FIL* fNameSort );
	int				sortForLenFileList					( const char* const path, uint16_t* fl, uint32_t countFileInDir, FILINFO* fi, DIR* d, FIL* fNoSort, FIL* fLenSort );
	int				removeSystemFileInRootDir			( const char* fatValome );
	int				getFileCountInCurDir				( FILE_LIST_TYPE listType, uint32_t& returnCount );
	void			playPauseSet( bool state );
	/// Проверяем наличие файла в директории 0 - нет, 1 есть, -1 флешке плохо.
	int checkingFileOrDir( const char* path, const char* nameFile, FILINFO* fi  );
	void stopPlayFile ( void );

	int removeFile( const  char* path, const char* nameFile );

	int removeDirRecurisve( const char* path, const char* nameDir );
	void volumeSet				( const uint8_t left, const uint8_t right);
	/// Рабочие окна приложения (отрисовки).
	/// Статусы дерева переходов ставятся через переменные состояния ниже.
	/// Окно воспроизведения трека
	void initPlayWindow ( void );
	void removePlayWindow ( void );

/*
	 * Находим системные файлы в директории.
	 * Например, карзина или индексер.
	 * Причем от разныех ОС.
*/
	int			checkingSystemFileInRootDir				( const char* fatValome );

	/// Заполняет внутренную переменную currentFileInfo
	/// данными из файла-списка на флешке.
	int			getFileInfoFromListCurDir ( FILE_LIST_TYPE listType, uint32_t numberFileInList );
	void		trackMainWindowInit					(	void	);



	void initEqualizer (	void	);


	void setValueEqualizer (	void	);


private:
	static void illuminationControlTask ( void* obj );
	static void buttonClickHandlerTask ( void* obj );
	static void playTickHandlerTask ( void* obj );
	static void playTask ( void* obj );

private:
	int fsmStepFuncHardwareMcInit ( void );
	int fsmStepFuncFreeRtosthisInit ( void );
	int fsmStepFuncGuiInit ( void );
	int fsmStepFuncMicroSdInit ( void );
	int fsmStepFuncIndexingSupportedFiles ( void );
	int fsmStepFuncSortingFileList ( void );
	int fsmStepFuncCheckingChangeFatVolume ( void );
	int fsmStepFuncCleanFlagChangeFatVolume ( void );
	int fsmStepFuncInitMainWindow ( void );

private:

	/// В объекте fat для sd1 должена быть установлена актуальная директория с помощью f_chdir
	/// Ну то есть мы указываем имя файла относительно текущей директории.
	int		startPlayFile							(	void	);
	void	startPlayTrack ( void );


	void	initAyCfgCall								(	void	);
	void	initEqualizerWindow							( void );
	void	removeEqualizerWindow						(	void	);

	int		ayFileCallOpenFile							(	void	);
	int		ayFileCallCloseFile							(	void	);
	int		ayFileCallGetFileLen						(	uint32_t&		returnFileLenByte	);
	int		ayFileCallSetOffsetByteInFile				(	const uint32_t 	offsetByte	);
	int		ayFileCallReadInArray						(	uint8_t*		returnDataBuffer,
															const uint32_t	countByteRead	);
	int		ayFileCallSetPwrChip						(	bool			state	);
	int		ayFileCallInitChip							(	void	);
	int		ayFileCallSleepChip							(	const uint32_t	countTick	);
	int		ayFileCallWritePacket						(	const uint8_t	reg,
															const uint8_t	data	);

	/// Текущий режим работы RCC.
	uint32_t											rccIndex = 0;

	const AyPlayerCfg* const cfg;

	USER_OS_STATIC_STACK_TYPE							tbMainTask[ TB_MAIN_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE						tsMainTask;
	USER_OS_STATIC_STACK_TYPE							tbIlluminationControlTask[ TB_ILLUMINATION_CONTROL_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE						tsIlluminationControlTask;
	USER_OS_STATIC_STACK_TYPE							tbButtonClickHandlerTask[ TB_BUTTON_CLICK_HANDLER_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE						tsButtonClickHandlerTask;
	USER_OS_STATIC_STACK_TYPE							tbPlayTask[ TB_PLAY_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE						tsPlayTask;
	USER_OS_STATIC_STACK_TYPE							tbPlayTickTask[ TB_PLAY_TICK_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE						tsPlayTickTask;

	USER_OS_TIMER										timNameScroll;
	USER_OS_TIMER_STATIC_STRUCT							timStNameScroll;

	AyPlayerModuleGui g;

	/// Яркость подсветки.
	float												illuminationDuty = 1;

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
};
