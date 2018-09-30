#include "base.h"

namespace AyPlayer {

void Base::playPauseSet( bool state ) {
	this->cfg->ay->setPause( state );

	if ( state ) {
		this->playState		=	AYPLAYER_STATUS::PLAY;
	} else {
		this->playState		=	AYPLAYER_STATUS::PAUSE;
	}
}

void Base::stopPlayFile ( void ) {
	this->cfg->ay->stop();
	this->playState		=	AYPLAYER_STATUS::STOP;
	this->gui->resetPlayBarInMainWindow();
}

#define PARAM(P)			switch( static_cast< uint32_t >( P ) )
#define CASE(C)				case static_cast< uint32_t >( C )

void Base::buttonClickInMainWindow ( EC_BUTTON_NAME b ) {
	if ( b == EC_BUTTON_NAME::ENTER_CLICK ) {
		PARAM( this->playState ) {
		CASE( AYPLAYER_STATUS::STOP ):
			USER_OS_GIVE_BIN_SEMAPHORE( this->cfg->os->sStartPlay );
			return;

		CASE( AYPLAYER_STATUS::PAUSE ):
			this->playPauseSet( true );
			this->gui->update();
			return;

		CASE( AYPLAYER_STATUS::PLAY ):
			this->playPauseSet( false );
			this->gui->update();
			return;
		};
	}
/*
	if ( b == EC_BUTTON_NAME::ENTER_LONG_PRESS ) {
		if ( ( this->playState == AYPLAYER_STATUS::PLAY ) || ( this->playState == AYPLAYER_STATUS::PAUSE ) ) {
			this->stopPlayFile();
			//this->guiUpdate();
		}
		continue;
	}

	if ( b == EC_BUTTON_NAME::LEFT_CLICK ) {
		if ( this->currentFile > 0 ) {
			this->currentFile--;
			this->stopPlayFile();
			this->startPlayTrack();
			mSlimHorizontalListLeft( &this->g.shl, this->fat.currentFileInfo.fileName );
			//this->guiUpdate();
		}
		continue;
	}

	if ( b == EC_BUTTON_NAME::RIGHT_CLICK ) {
		if ( this->currentFile < this->countFileInCurrentDir - 1 ) {
			this->currentFile++;
			this->stopPlayFile();
			this->startPlayTrack();
			mSlimHorizontalListRight( &this->g.shl, this->fat.currentFileInfo.fileName );
			//this->guiUpdate();
		}
		continue;
	}

	/// Переход в эквалайзер.
	if ( b == EC_BUTTON_NAME::DOWN_CLICK ) {
		this->removePlayWindow();									/// Закрываем текущее окно.
		this->wNow = AYPLAYER_WINDOW_NOW::EQUALIZER;				/// Говорим что следующее будет эквалайзер.
		this->initEqualizerWindow();
		//this->guiUpdate();
		continue;
	}*/
}

void Base::buttonClickHandlerTask (  void* obj  ) {
	EC_BUTTON_NAME		b;
	uint8_t				qData;
	Base* o =( Base* ) obj;
	while( true ) {
		/// Старые команды нас не интересуют
		/// (то, что нажали за время выполнения предыдущей).
		USER_OS_QUEUE_RESET( o->cfg->os->qAyButton );
		USER_OS_QUEUE_RECEIVE( o->cfg->os->qAyButton, &qData, portMAX_DELAY );

		b	=	static_cast< EC_BUTTON_NAME >( qData );

		if ( o->wNow == AYPLAYER_WINDOW_NOW::MAIN ) {
			o->buttonClickInMainWindow( b );
		}
	}
}

}



/// * Окно эквалайзера.
/*
if ( o->wNow == AYPLAYER_WINDOW_NOW::EQUALIZER ) {
	if ( ( b == EC_BUTTON_NAME::BACK_LONG_CLICK ) ||
		 ( b == EC_BUTTON_NAME::BACK_CLICK ) ) {
		o->removeEqualizerWindow();
		o->wNow = AYPLAYER_WINDOW_NOW::MAIN;
		o->initPlayWindow();
		//o->guiUpdate();
		continue;
	}

	if ( ( b == EC_BUTTON_NAME::LEFT_LONG_CLICK ) ||
		 ( b == EC_BUTTON_NAME::LEFT_CLICK ) ) {

		o->g.currentSlider--;
		if ( o->g.currentSlider < 0 )
			o->g.currentSlider	=	5;

		mi_focus_prev( &makiseHost );
		//o->guiUpdate();
		continue;
	}

	if ( ( b == EC_BUTTON_NAME::RIGHT_LONG_CLICK ) ||
		 ( b == EC_BUTTON_NAME::RIGHT_CLICK ) ) {

		o->g.currentSlider++;
		if ( o->g.currentSlider == 6)
			o->g.currentSlider	=	0;

		mi_focus_next( &makiseHost );
		//o->guiUpdate();
		continue;
	}

	if ( ( b == EC_BUTTON_NAME::DOWN_CLICK ) ||
		 ( b == EC_BUTTON_NAME::DOWN_LONG_CLICK ) ) {
		uint8_t* value;
		value = &o->eq.A1;

		if ( value[ o->g.currentSlider ] > 0 ) {
			int b = value[ o->g.currentSlider ] - 10;
			if ( b < 0 ) {
				value[ o->g.currentSlider ] = 0;
			} else {
				value[ o->g.currentSlider ] = static_cast< uint8_t >( b );
			}
		}

		m_slider_set_value(	o->g.sliders[ o->g.currentSlider ],
							value[ o->g.currentSlider ] );

		o->setValueEqualizer();

		//o->guiUpdate();
		continue;
	}

	if ( ( b == EC_BUTTON_NAME::UP_CLICK ) ||
		 ( b == EC_BUTTON_NAME::UP_LONG_CLICK ) ) {
		uint8_t* value;
		value = &o->eq.A1;

		if ( value[ o->g.currentSlider ] < 0xFF ) {
			int b = value[ o->g.currentSlider ] + 10;
			if ( b > 0xFF ) {
				value[ o->g.currentSlider ] = 0xFF;
			} else {
				value[ o->g.currentSlider ] = static_cast< uint8_t >( b );
			}
		}

		m_slider_set_value(	o->g.sliders[ o->g.currentSlider ],
							value[ o->g.currentSlider ] );

		o->setValueEqualizer();

		//o->guiUpdate();
		continue;
	}
}*/

