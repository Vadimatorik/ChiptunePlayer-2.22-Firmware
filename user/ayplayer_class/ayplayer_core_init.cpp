#include "ayplayer.h"
#include "core_cm4.h"
#include "ayplayer_os_object.h"

int AyPlayer::fsmStepFuncHardwareMcInit ( void ) {
	BaseResult r;

	/*!
	 * WDT init.
	 */
	//r = this->cfg->mcu->wdt->reinit( 0 );
	//assertParam( r == BASE_RESULT::OK );

	this->cfg->mcu->pwr->reinit( 0 );

	/*!
	 * GPIO init.
	 */
	///
	r = this->cfg->mcu->gp->reinitAllPorts();
	assertParam( r == BaseResult::ok );

	/// Фиксируем питание.
	this->cfg->mcu->gpio->pwr.allPwr->reset();

	/*!
	 * RCC и все объекты, зависящие от него.
	 */
	this->rccMaxFrequancyInit();

	/*!
	 * NVIC.
	 */
	NVIC_SetPriority( DMA1_Stream3_IRQn, 10 );
	NVIC_SetPriority( DMA1_Stream4_IRQn, 10 );
	NVIC_SetPriority( DMA1_Stream7_IRQn, 5 );				/// Выдача в AY (сдвиговый регистр).
	NVIC_SetPriority( DMA2_Stream5_IRQn, 10 );
	NVIC_SetPriority( DMA2_Stream6_IRQn, 10 );

	NVIC_SetPriority( USART3_IRQn, 10 );

	NVIC_SetPriority( PVD_IRQn, 10 );

	/// Выдача семафора задаче обновления AY регистров.
	NVIC_SetPriority( TIM6_DAC_IRQn, 5 );

	NVIC_EnableIRQ( DMA1_Stream3_IRQn );
	NVIC_EnableIRQ( DMA1_Stream4_IRQn );
	NVIC_EnableIRQ( DMA1_Stream7_IRQn );
	NVIC_EnableIRQ( DMA2_Stream5_IRQn );
	NVIC_EnableIRQ( DMA2_Stream6_IRQn );

	NVIC_EnableIRQ( USART3_IRQn );

	NVIC_EnableIRQ( PVD_IRQn );

	NVIC_EnableIRQ( TIM6_DAC_IRQn );

	/*!
	 * После инициализации запускаем все модули,
	 * которые должны всегда находиться в работе.
	 */
	this->startBaseInterfaces();
	return 0;
}

int AyPlayer::fsmStepFuncFreeRtosthisInit ( void ) {
	this->cfg->os->qAyLow[0]		=	USER_OS_STATIC_QUEUE_CREATE( QB_AY_LOW_SIZE, sizeof( ayLowOutDataStruct ), &this->cfg->os->qbAyLow[0][0], &this->cfg->os->qsAyLow[0] );
	this->cfg->os->qAyLow[1]		=	USER_OS_STATIC_QUEUE_CREATE( QB_AY_LOW_SIZE, sizeof( ayLowOutDataStruct ), &this->cfg->os->qbAyLow[1][0], &this->cfg->os->qsAyLow[1] );
	this->cfg->os->qAyButton		=	USER_OS_STATIC_QUEUE_CREATE( 1, sizeof( uint8_t ), this->cfg->os->qbAyButton, &this->cfg->os->qsAyButton );

	this->cfg->os->sPlayTic		=	USER_OS_STATIC_BIN_SEMAPHORE_CREATE( &this->cfg->os->sbPlayTic );
	this->cfg->os->sStartPlay		=	USER_OS_STATIC_BIN_SEMAPHORE_CREATE( &this->cfg->os->sbStartPlay );

	this->cfg->os->mHost			=	USER_OS_STATIC_MUTEX_CREATE( &this->cfg->os->mbHost );

	return 0;
}

extern "C" {

extern MHost				makiseHost;
extern MakiseGUI			makiseGui;
extern MakiseDriver			makiseGuiDriver;

uint32_t*	makiseGuiGetBuffer			( uint32_t size );
void		makiseGuiPredraw			( MakiseGUI* gui );
void		makiseGuiDraw				( MakiseGUI* gui );
MInputData	makiseGuiInpHandler			( MInputData d, MInputResultEnum res );
void		makiseGuiUpdate				( MakiseGUI* gui );

}

extern ST7565		lcd;

int AyPlayer::fsmStepFuncGuiInit ( void ) {
	makise_gui_autoinit(	&makiseHost,
							&makiseGui,
							&makiseGuiDriver,
							makiseGuiGetBuffer,
							makiseGuiInpHandler,
							makiseGuiDraw,
							makiseGuiPredraw,
							makiseGuiUpdate		);

	/// Статус бар всегда показывается.
	this->initGuiStatusBar();

	if ( lcd.reset()			!= BaseResult::ok ) return M_ERROR;
	if ( lcd.setContrast( 8 )	!= BaseResult::ok ) return M_ERROR;
	if ( lcd.clear()			!= BaseResult::ok ) return M_ERROR;
	if ( lcd.on()				!= BaseResult::ok ) return M_ERROR;



	this->cfg->l->sendMessage( RTL_TYPE_M::INIT_OK, "MakiseGui started." );

	return 0;
}

