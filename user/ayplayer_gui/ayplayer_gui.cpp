#include "ayplayer_gui.h"

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

namespace AyPlayer {

Gui::Gui	(	const AyPlayerPcbStrcut*				const pcbObj,
				const AyPlayerGuiModuleStyleCfg*		const cfg	) :
						pcbObj( pcbObj ), cfg( cfg )	{
}

void Gui::init ( void ) {
	makise_gui_autoinit(	&makiseHost,
							&makiseGui,
							&makiseGuiDriver,
							makiseGuiGetBuffer,
							makiseGuiInpHandler,
							makiseGuiDraw,
							makiseGuiPredraw,
							makiseGuiUpdate		);

	this->addStatusBar();

	McHardwareInterfaces::BaseResult	r;

	r = this->pcbObj->lcd->reset();
	this->checkAndExit( r );
	r = this->pcbObj->lcd->setContrast( 8 );
	this->checkAndExit( r );
	r = this->pcbObj->lcd->lcdClear();
	this->checkAndExit( r );
	r = this->pcbObj->lcd->on();
	this->checkAndExit( r );
}

void Gui::addStatusBar( void ) {
	m_create_player_status_bar(	&this->g.sb,
								&makiseHost.host,
								mp_rel(	0,	0,
										128, 12	),
								&this->cfg->statusBarCfg,
								&this->cfg->statusBarCallbackCfg	);
}

void Gui::checkAndExit ( McHardwareInterfaces::BaseResult resultValue ) {
	if ( resultValue != McHardwareInterfaces::BaseResult::ok ) {
		AyPlayer::Nvic::reboot();
	}
}

}
