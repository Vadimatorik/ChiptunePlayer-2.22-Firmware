#include "gui.h"

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

#define	TEST_POINT_AND_DELITE(POINT)			\
			if ( POINT ) {						\
				delete POINT;					\
				POINT			=	nullptr;	\
			}

void Gui::removeOldWindow	( void ) {
	USER_OS_STATIC_TIMER_STOP( this->timStringScroll );

	TEST_POINT_AND_DELITE( this->pb );
	TEST_POINT_AND_DELITE( this->wisf );
	TEST_POINT_AND_DELITE( this->shl );
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

	this->timStringScroll	= USER_OS_STATIC_TIMER_CREATE(	"scrollStringCurrentNameTrack",
																this->timerSpeedLow,
																this,
																Gui::timerHandler,
																&this->timStStringScroll	);

	McHardwareInterfaces::BaseResult	r;

	r = this->pcbObj->lcd->reset();
	this->checkAndExit( r );
	r = this->pcbObj->lcd->setContrast( 8 );
	this->checkAndExit( r );
	r = this->pcbObj->lcd->lcdClear();
	this->checkAndExit( r );
	r = this->pcbObj->lcd->on();
	this->checkAndExit( r );

	this->statusBar	=	this->addStatusBar();

	USER_OS_STATIC_TASK_CREATE(	illuminationControlTask,
								"illuminationControl",
								this->tbIlluminationControlTaskSize,
								( void* )this,
								this->illuminationControlTaskPrio,
								this->tbIlluminationControlTask,
								&this->tsIlluminationControlTask	);
}

void Gui::update ( void ) {
	this->updateWithoutLed();
	USER_OS_GIVE_BIN_SEMAPHORE( this->sUpdateLcd );
}

void Gui::updateWithoutLed ( void ) {
	McHardwareInterfaces::BaseResult r;

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

WindowMessage* Gui::addWindowMessage ( const char*	const message ) {
	return new WindowMessage( message, &this->cfg->smw, &makiseHost.host, 0,	11, 128, 64 - 11 );
}

StatusBar* Gui::addStatusBar( void ) {
	return new StatusBar(	&this->cfg->statusBarCfg,
							&this->cfg->statusBarCallbackCfg,
							&makiseHost.host,
							0,	0,	128, 12			);
}



HorizontalList* Gui::addHorizontalList ( void ) {
	return new HorizontalList(	&this->cfg->horizontalListStyle,
								&makiseHost.host,
								0,	11,	128, 12	);
}

PlayBar* Gui::addPlayBar ( void ) {
	return new PlayBar( &this->cfg->playBarStyle,
						&makiseHost.host,
						0,	57,	128, 7	);
}






void	Gui::setWindowIndexingSupportedFiles		(	WindowIndexingSupportedFiles**		returnWisfObj	) {
	this->removeOldWindow();
	this->wisf = new WindowIndexingSupportedFiles(	&this->cfg->ssl,
													&this->cfg->sslItem,
													&makiseHost.host,
													0,	11,	128, 64 - 11	);
	*returnWisfObj = this->wisf;
}

void Gui::setWindowMain	(	std::shared_ptr< ItemFileInFat >	item,
							uint32_t							countItems	) {
	USER_OS_STATIC_TIMER_STOP( this->timStringScroll );

	this->removeOldWindow();
	this->pb		=	this->addPlayBar();
	this->pb->setLenInTick( item->lenTick );
	this->shl		=	this->addHorizontalList();
	this->shl->setItemCount( countItems );
	this->shl->setStringCurrentItem( item->fileName );

	USER_OS_STATIC_TIMER_CHANGE_PERIOD( this->timStringScroll, this->timerSpeedLow );
	USER_OS_STATIC_TIMER_START( this->timStringScroll );
}

void Gui::updateTreckWindowMain	(	std::shared_ptr< ItemFileInFat >	item	) {
	USER_OS_STATIC_TIMER_STOP( this->timStringScroll );

	this->pb->setLenInTick( item->lenTick );
	this->shl->setStringCurrentItem( item->fileName );

	USER_OS_STATIC_TIMER_CHANGE_PERIOD( this->timStringScroll, this->timerSpeedLow );
	USER_OS_STATIC_TIMER_START( this->timStringScroll );
}

void	Gui::resetPlayBarInMainWindow	(	void	) {
	this->pb->reset();
}

void	Gui::incTickPlayBarInMainWindow	(	void	) {
	this->pb->inc();
}


void	Gui::timerHandler	(	TimerHandle_t		timer	) {
	Gui* o = reinterpret_cast< Gui* >( pvTimerGetTimerID( timer ) );

	int r;
	r = o->shl->scroll();

	if ( r != 1 ) {
		if ( USER_OS_STATIC_TIMER_GET_PERIOD( o->timStringScroll ) == o->timerSpeedLow	) {
			USER_OS_STATIC_TIMER_CHANGE_PERIOD( o->timStringScroll, o->timerSpeedFast );
		}
	} else {
		USER_OS_STATIC_TIMER_CHANGE_PERIOD( o->timStringScroll, o->timerSpeedLow );
	}

	o->updateWithoutLed();
}

}
