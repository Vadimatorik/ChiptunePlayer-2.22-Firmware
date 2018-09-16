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

WindowMessage::WindowMessage(	const char*						const message,
								const MakiseStyle_SMessageWindow*		style,
								MContainer*						c,
								int32_t							x,
								int32_t							y,
								uint32_t						w,
								uint32_t						h				) {
	uint32_t		l	=	strlen( message );
	this->str			=	new char[ l + 1 ];
	this->str[ l ]		=	0;

	strcpy( this->str, message );

	m_create_message_window(	&this->mw,
								c,
								mp_rel( x, y, w, h ),
								this->str,
								style );
}

WindowMessage::~WindowMessage() {
	delete this->str;
	makise_g_cont_rem( &this->mw.el );
}

StatusBar::StatusBar	(	const MakiseStyle_SMPlayerStatusBar*	style,
							const MPlayerStatusBar_CallbackFunc* 	callbacks,
							MContainer*								c,
							int32_t									x,
							int32_t									y,
							uint32_t								w,
							uint32_t								h	) {
	m_create_player_status_bar(	&this->sb,
								c,
								mp_rel(	x, y, w, h	),
								style,
								callbacks	);
}

StatusBar::~StatusBar() {
}

WindowIndexingSupportedFiles::WindowIndexingSupportedFiles	(	const MakiseStyle_SList*		style,
																const MakiseStyle_SListItem*	itemsStyle,
																MContainer*						c,
																int32_t							x,
																int32_t							y,
																uint32_t						w,
																uint32_t						h				) {
	this->str	=	new char[ 30 ];

	m_create_slist(	&this->sl,
					c,
					mp_rel( x, y, w, h ),
					this->str,
					nullptr,
					nullptr,
					MSList_List,
					style,
					itemsStyle );

	/// Привязка list-а к его элементам.
	for ( int i = 0; i < 4; i++ ) {
		this->slItem[ i ].text	=	nullptr;
		m_slist_add( &this->sl, &this->slItem[ i ] );
	}
}

void WindowIndexingSupportedFiles::setActualState ( const char* const stateMessage ) {
	memcpy( this->str, stateMessage, 30 );
}

// Метод сдвигает вниз все строки (1 удаляется) и добавляет вверх новую.
void WindowIndexingSupportedFiles::addItem ( const char* const itemName ) {
	/// Если раньше там была не пустая строка.
	if ( this->slItem[ this->cout - 1 ].text != nullptr ) {
		delete[] this->slItem[  this->cout - 1 ].text;
	}

	for ( uint32_t l =  this->cout - 1; l > 0 ; l-- ) {
		this->slItem[ l ].text = this->slItem[ l - 1 ].text;
	}

	uint32_t lenString = strlen( itemName ) + 1;
	this->slItem[ 0 ].text = new char[ lenString ];
	strcpy( this->slItem[ 0 ].text, itemName );
}

WindowIndexingSupportedFiles::~WindowIndexingSupportedFiles() {
	delete[] this->str;
	makise_g_cont_rem( &this->sl.el );
}






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

	this->statusBar = this->addStatusBar();

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

WindowMessage* Gui::addWindowMessage ( const char*	const message ) {
	return new WindowMessage( message, &this->cfg->smw, &makiseHost.host, 0,	11, 128, 64 - 11 );
}

StatusBar* Gui::addStatusBar( void ) {
	return new StatusBar(	&this->cfg->statusBarCfg,
							&this->cfg->statusBarCallbackCfg,
							&makiseHost.host,
							0,	0,	128, 12			);
}

WindowIndexingSupportedFiles* Gui::addWindowIndexingSupportedFiles ( void ) {
	return new WindowIndexingSupportedFiles(	&this->cfg->ssl,
												&this->cfg->sslItem,
												&makiseHost.host,
												0,	11,	128, 64 - 11	);
}

}
