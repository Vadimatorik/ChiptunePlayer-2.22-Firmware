#include "ayplayer.h"
#include "core_cm4.h"
#include "ayplayer_os_object.h"

namespace AyPlayer {

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

extern MonoLcd::ST7565		lcd;

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

	if ( lcd.reset()			!= McHardwareInterfaces::BaseResult::ok ) return M_ERROR;
	if ( lcd.setContrast( 8 )	!= McHardwareInterfaces::BaseResult::ok ) return M_ERROR;
	if ( lcd.lcdClear()			!= McHardwareInterfaces::BaseResult::ok ) return M_ERROR;
	if ( lcd.on()				!= McHardwareInterfaces::BaseResult::ok ) return M_ERROR;



	this->cfg->l->sendMessage( RTL_TYPE_M::INIT_OK, "MakiseGui started." );

	return 0;
}

}
