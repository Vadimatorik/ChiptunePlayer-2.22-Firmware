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
				const AyPlayerGuiModuleStyleCfg*		const cfg,
				McHardwareInterfaces::TimPwmOneChannel*	ledPwm	) :
						pcbObj( pcbObj ), cfg( cfg ), ledPwm( ledPwm )	{
	this->mHost			=	USER_OS_STATIC_MUTEX_CREATE( &this->mbHost );
	this->sUpdateLcd	=	USER_OS_STATIC_BIN_SEMAPHORE_CREATE( &this->sbUpdateLcd );
}

void Gui::illuminationControlTask ( void*	obj ) {
	Gui* o =( Gui* ) obj;
	o->ledPwm->setDuty( o->maxIlluminationDuty );
	o->ledPwm->on();

	uint32_t	downCounterTime	=	o->maxIlluminationTimeS;
	bool		flagMode		=	true;

	while( true ) {
		if ( USER_OS_TAKE_BIN_SEMAPHORE ( o->sUpdateLcd, 1000 ) == pdTRUE ) {
			downCounterTime =	o->maxIlluminationTimeS;
			o->ledPwm->setDuty( o->maxIlluminationDuty );
			o->ledPwm->on();
			flagMode		=	true;
		} else {
			downCounterTime--;
			if ( downCounterTime == 0 ) {
				if ( flagMode ) {
					flagMode	=	0;
					downCounterTime =	o->minIlluminationTimeS;
					o->ledPwm->setDuty( o->minIlluminationDuty );
				} else {
					o->ledPwm->setDuty( 0 );
					o->ledPwm->off();
				}
			}
		}
	}
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

	USER_OS_STATIC_TASK_CREATE(	illuminationControlTask,
								"illuminationControl",
								this->tbIlluminationControlTaskSize,
								( void* )this,
								this->illuminationControlTaskPrio,
								this->tbIlluminationControlTask,
								&this->tsIlluminationControlTask	);
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

	USER_OS_GIVE_BIN_SEMAPHORE( this->sUpdateLcd );

	USER_OS_GIVE_MUTEX( this->mHost );
}

void Gui::checkAndExit ( McHardwareInterfaces::BaseResult resultValue ) {
	if ( resultValue != McHardwareInterfaces::BaseResult::ok ) {
		AyPlayer::Nvic::reboot();
	}
}

void Gui::setMaxIlluminationDuty ( float	maxIlluminationDuty ) {
	if ( maxIlluminationDuty > 1 ) {
		this->maxIlluminationDuty	=	1;
	} else if ( maxIlluminationDuty < 0 ) {
		this->maxIlluminationDuty	=	0;
	} else {
		this->maxIlluminationDuty	=	maxIlluminationDuty;
	}
}

void Gui::setMinIlluminationDuty ( float	minIlluminationDuty ) {
	if ( minIlluminationDuty > 1 ) {
		this->minIlluminationDuty	=	1;
	} else if ( minIlluminationDuty < 0 ) {
		this->minIlluminationDuty	=	0;
	} else {
		this->minIlluminationDuty	=	minIlluminationDuty;
	}
}

void Gui::setMaxIlluminationTime ( uint32_t		maxIlluminationTimeS ) {
	this->maxIlluminationTimeS	=	maxIlluminationTimeS;
}

void Gui::setMinIlluminationTime ( uint32_t		minIlluminationTimeS ) {
	this->minIlluminationTimeS	=	minIlluminationTimeS;
}

void Gui::addMessage ( const char*	const message ) {
	m_create_message_window(	&this->g.mw,
								&makiseHost.host,
								mp_rel( 0,	11,
										128, 64 - 11 ),
								( char* )message,
								( MakiseStyle_SMessageWindow* )&this->cfg->smw );
}

void Gui::removeMessage (	void	) {
	makise_g_cont_rem( &this->g.mw.el );
}

}
