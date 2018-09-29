#include "ayplayer_fat_error_string.h"
#include "base.h"

#include "makise_gui.h"
#include "makise.h"

extern "C" {

extern MHost				makiseHost;
extern MakiseGUI			makiseGui;
extern MakiseDriver			makiseGuiDriver;

}

namespace AyPlayer {

/*
void Base::initPlayWindow ( void ) {
	mCreatePlayBar(	&this->g.pb,
					&makiseHost.host,
					mp_rel(	0,	57,
							128, 7	),
					1,
					&this->cfg->gui->playBarStyle );

	/// Время трека сразу в статус бар.
	mPlayBarSetNewTrack( &this->g.pb, this->fat.currentFileInfo.lenTick / 50 );

	mCreateSlimHorizontalList(	&this->g.shl,
								&makiseHost.host,
								mp_rel(	0,	11,
										128, 12	),
								&this->cfg->gui->horizontalListStyle	);

	// Запускаем скролл строки трека в главном
	 // окне и инициализируем все элементы.
	this->trackMainWindowInit();

	/// Указываем колличество элементов в директории.
	mSlimHorizontalSetItemCount( &this->g.shl, this->countFileInCurrentDir );
}

void Base::removePlayWindow ( void ) {
	USER_OS_STATIC_TIMER_STOP( this->timNameScroll );	/// Скролить строку теперь не нужно.

	makise_g_cont_rem( &this->g.pb.el );
	makise_g_cont_rem( &this->g.shl.el );
}


void Base::initEqualizerWindow ( void ) {
	uint32_t	x		=	5 ;							/// 5 отступ с каждой стороны.
	uint32_t	w		=	18;							/// Ширина 18  на каждый слайдер.
	uint32_t	step	=	2;							/// 2 между.
	uint32_t	y		=	12 + 4;						/// Отступаем сверху статус бар и 4 пикселя.
	uint32_t	h		=	( 64 - 1 ) - 12 - 8 - 8;	/// Формируем высоту.

	for ( int i = 0; i < 6; i++ ) {
		this->g.sliders[ i ]	=	( MSlider* )pvPortMalloc( sizeof( MSlider ) );
		assertParam( &this->g.sliders[ i ] );

		m_create_slider(	this->g.sliders[ i ],
							&makiseHost.host,
							mp_rel(	x,	y,
									w,	h	),
							MSlider_Type_ReadWrite,
							&this->cfg->gui->m	);

		m_slider_set_range( this->g.sliders[ i ], 0, 255 );

		m_slider_set_value(	this->g.sliders[ i ],
							*( ( ( uint8_t* )&this->eq.A1 ) + i ) );

		x += w + step;
	}

	mi_focus( &this->g.sliders[ 0 ]->el, M_G_FOCUS_GET );
	this->g.currentSlider	=	0;
}

void Base::removeEqualizerWindow ( void ) {
	for ( int i = 0; i < 6; i++ ) {
		makise_g_cont_rem( &this->g.sliders[ i ]->el );
		vPortFree( this->g.sliders[ i ] );
	}
}

void Base::errorMicroSdDraw ( const FRESULT r ) {
	char	massage[ 100 ];

	massage[ 0 ] = 'S';
	massage[ 1 ] = 'D';

	massage[ 2 ] = ':';

	const char*	s;

	switch ( r ) {
		case FR_DISK_ERR:				s	=	sErrDiskErr;				break;
		case FR_INT_ERR:				s	=	sErrIntErr;					break;
		case FR_NOT_READY:				s	=	sErrNotReady;				break;
		case FR_NO_FILE:				s	=	sErrNoFile;					break;
		case FR_NO_PATH:				s	=	sErrnoPath;					break;
		case FR_INVALID_NAME:			s	=	sErrInvalidName;			break;
		case FR_DENIED:					s	=	sErrDenied;					break;
		case FR_EXIST:					s	=	sErrExist;					break;
		case FR_INVALID_OBJECT:			s	=	sErrInvalidObject;			break;
		case FR_WRITE_PROTECTED:		s	=	sErrWriteProtected;			break;
		case FR_INVALID_DRIVE:			s	=	sErrInvalidDrive;			break;
		case FR_NOT_ENABLED:			s	=	sErrNotEnabled;				break;
		case FR_NO_FILESYSTEM:			s	=	sErrnoFilesystem;			break;
		case FR_MKFS_ABORTED:			s	=	sErrMkfsAborted;			break;
		case FR_TIMEOUT:				s	=	sErrTimeout;				break;
		case FR_LOCKED:					s	=	sErrLocked;					break;
		case FR_NOT_ENOUGH_CORE:		s	=	sErrNotEnoughCore;			break;
		case FR_TOO_MANY_OPEN_FILES:	s	=	sErrTooManyOpenFiles;		break;
		case FR_INVALID_PARAMETER:		s	=	sErrInvalidParameter;		break;

		default:
			while( 1 );	/// Невозможный исход.
		}


		strcpy( &massage[ 4 ], s );
		massage[ 3 ]	=	'\t';							/// В консоле удобнее через tab.
		this->cfg->l->sendMessage( RTL_TYPE_M::INIT_ISSUE, massage );

		massage[ 3 ]	=	' ';							/// На экране однозначно на разных строках.

		/// Создаем временный граф. объект.
		MMessageWindow*			m;
		m	=	( MMessageWindow* )pvPortMalloc( sizeof( MMessageWindow ) );
		assertParam( m );

		m_create_message_window(	m,
									&makiseHost.host,
									mp_rel( 0,	11,
											128, 64 - 11 ),
									massage,
									&this->cfg->gui->smw );
		//this->guiUpdate();

		makise_g_cont_rem( &m->el );
		vPortFree( m );
}

void Base::slItemClean ( uint32_t cout ) {
	for ( uint32_t l = 0; l < cout ; l++ ) {
		if ( this->g.slItem[ l ]->text ) {
			vPortFree( this->g.slItem[ l ]->text );
		}
	}
}*/




}

