#include "ayplayer.h"
#include "ayplayer_button.h"

namespace AyPlayer {



AyPlayer::AyPlayer ( const AyPlayerCfg* const cfg ) : cfg( cfg ) {
	this->rcc						=	new Rcc( this->cfg->mcu );

	this->cfg->os->qAyLow[0]		=	USER_OS_STATIC_QUEUE_CREATE( QB_AY_LOW_SIZE, sizeof( ayLowOutDataStruct ), &this->cfg->os->qbAyLow[0][0], &this->cfg->os->qsAyLow[0] );
	this->cfg->os->qAyLow[1]		=	USER_OS_STATIC_QUEUE_CREATE( QB_AY_LOW_SIZE, sizeof( ayLowOutDataStruct ), &this->cfg->os->qbAyLow[1][0], &this->cfg->os->qsAyLow[1] );
	this->cfg->os->qAyButton		=	USER_OS_STATIC_QUEUE_CREATE( 1, sizeof( uint8_t ), this->cfg->os->qbAyButton, &this->cfg->os->qsAyButton );

	this->cfg->os->sPlayTic			=	USER_OS_STATIC_BIN_SEMAPHORE_CREATE( &this->cfg->os->sbPlayTic );
	this->cfg->os->sStartPlay		=	USER_OS_STATIC_BIN_SEMAPHORE_CREATE( &this->cfg->os->sbStartPlay );}

void AyPlayer::initTasks ( void ) {
	USER_OS_STATIC_TASK_CREATE(	AyPlayer::mainTask,
								"main",
								TB_MAIN_TASK_SIZE,
								( void* )this,
								MAIN_TASK_PRIO,
								this->tbMainTask,
								&this->tsMainTask );
}

void AyPlayer::start ( void ) {
	this->initHardwareMc();
	this->initTasks();
	vTaskStartScheduler();

	/// Основной поток проекта.


/*
	Контроль подсветки экрана.

	USER_OS_STATIC_TASK_CREATE(	AyPlayer::illuminationControlTask,
								"illuminationControl",
								TB_ILLUMINATION_CONTROL_TASK_SIZE,
								( void* )this,
								ILLUMINATION_CONTROL_TASK_PRIO,
								this->tbIlluminationControlTask,
								&this->tsIlluminationControlTask	);

	/// Обработка нажатой клавиши.
	USER_OS_STATIC_TASK_CREATE(	AyPlayer::buttonClickHandlerTask,
								"buttonClickHandler",
								TB_BUTTON_CLICK_HANDLER_TASK_SIZE,
								( void* )this,
								BUTTON_CLICK_HANDLER_TASK_PRIO,
								this->tbButtonClickHandlerTask,
								&this->tsButtonClickHandlerTask	);

	/// Воспроизведение трека.
	USER_OS_STATIC_TASK_CREATE(	AyPlayer::playTask,
								"play",
								TB_PLAY_TASK_SIZE,
								( void* )this,
								PLAY_TASK_PRIO,
								this->tbPlayTask,
								&this->tsPlayTask	);

	/// Обновление плей-бара при прохождении секунды.
	USER_OS_STATIC_TASK_CREATE(	AyPlayer::playTickHandlerTask,
								"playTick",
								TB_PLAY_TICK_TASK_SIZE,
								( void* )this,
								PLAY_TICK_TASK_PRIO,
								this->tbPlayTickTask,
								&this->tsPlayTickTask	);

	/// Прокрутка строки.
	this->timNameScroll = USER_OS_STATIC_TIMER_CREATE(	"ScrollStringName",
														1000,
														( void* )this,
														AyPlayer::scrollNameInMainWindow,
														&this->timStNameScroll	);

	USER_OS_STATIC_TIMER_START( this->timNameScroll );

	*/
}

void AyPlayer::checkAndExit ( McHardwareInterfaces::BaseResult resultValue ) {
	if ( resultValue != McHardwareInterfaces::BaseResult::ok ) {
		this->nvic.reboot();
	}
}

}

