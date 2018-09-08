#include "ayplayer.h"
#include "core_cm4.h"
#include "ayplayer_os_object.h"

namespace AyPlayer {

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
