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
	this->mHost			=	USER_OS_STATIC_MUTEX_CREATE( &this->mbHost );

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

void Gui::update ( void ) {
	McHardwareInterfaces::BaseResult	r;

	USER_OS_TAKE_MUTEX( this->mHost, portMAX_DELAY );

	this->pcbObj->lcd->bufClear();

	makise_g_host_call( &makiseHost, &makiseGui, M_G_CALL_PREDRAW );
	makise_g_host_call( &makiseHost, &makiseGui, M_G_CALL_DRAW );

	r = this->pcbObj->lcd->update();
	this->checkAndExit( r );

	USER_OS_GIVE_MUTEX( this->mHost );
}

void Gui::checkAndExit ( McHardwareInterfaces::BaseResult resultValue ) {
	if ( resultValue != McHardwareInterfaces::BaseResult::ok ) {
		AyPlayer::Nvic::reboot();
	}
}

}
