#include "base.h"

#include "ayplayer_button.h"

namespace AyPlayer {



Base::Base ( const BaseCfg* const cfg ) : cfg( cfg ) {
	this->rcc						=	new Rcc( this->cfg->mcu );
	this->gui						=	new Gui( this->cfg->pcb, this->cfg->gui, this->cfg->mcu->lcdPwmTim );
	this->sd						=	new SdControl( this->cfg->mcu->gpio->sd.it, this->cfg->mcu->gpio->sd.read, this->cfg->mcu->gpio->sd.set );

	this->cfg->os->qAyLow[0]		=	USER_OS_STATIC_QUEUE_CREATE( QB_AY_LOW_SIZE, sizeof( ayLowOutDataStruct ), &this->cfg->os->qbAyLow[0][0], &this->cfg->os->qsAyLow[0] );
	this->cfg->os->qAyLow[1]		=	USER_OS_STATIC_QUEUE_CREATE( QB_AY_LOW_SIZE, sizeof( ayLowOutDataStruct ), &this->cfg->os->qbAyLow[1][0], &this->cfg->os->qsAyLow[1] );
	this->cfg->os->qAyButton		=	USER_OS_STATIC_QUEUE_CREATE( 1, sizeof( uint8_t ), this->cfg->os->qbAyButton, &this->cfg->os->qsAyButton );

	this->cfg->os->sPlayTic			=	USER_OS_STATIC_BIN_SEMAPHORE_CREATE( &this->cfg->os->sbPlayTic );
	this->cfg->os->sStartPlay		=	USER_OS_STATIC_BIN_SEMAPHORE_CREATE( &this->cfg->os->sbStartPlay );

	this->cfg->os->mBoardSpi		=	USER_OS_STATIC_MUTEX_CREATE( &this->cfg->os->mbBoardSpi );
}

void Base::initTasks ( void ) {
	USER_OS_STATIC_TASK_CREATE(	Base::mainTask,
								"main",
								TB_MAIN_TASK_SIZE,
								( void* )this,
								MAIN_TASK_PRIO,
								this->tbMainTask,
								&this->tsMainTask );

	USER_OS_STATIC_TASK_CREATE(	Base::buttonClickHandlerTask,
								"ButtonClickHandlerTask",
								TB_BUTTON_CLICK_HANDLER_TASK_SIZE,
								( void* )this,
								BUTTON_CLICK_HANDLER_TASK_PRIO,
								this->tbButtonClickHandlerTask,
								&this->tsButtonClickHandlerTask );

	USER_OS_STATIC_TASK_CREATE(	Base::playTask,
								"Play",
								TB_PLAY_TASK_SIZE,
								( void* )this,
								PLAY_TASK_PRIO,
								this->tbPlayTask,
								&this->tsPlayTask	);

	USER_OS_STATIC_TASK_CREATE(	Base::playTickHandlerTask,
								"PlayTickHandler",
								TB_PLAY_TICK_TASK_SIZE,
								( void* )this,
								PLAY_TICK_TASK_PRIO,
								this->tbPlayTickTask,
								&this->tsPlayTickTask	);
}

void Base::start ( void ) {
	this->initHardwareMc();
	this->initTasks();
	vTaskStartScheduler();
}

void Base::checkAndExit ( mc_interfaces::res resultValue ) {
	if ( resultValue != mc_interfaces::res::err_ok ) {
		this->nvic.reboot();
	}
}

}

