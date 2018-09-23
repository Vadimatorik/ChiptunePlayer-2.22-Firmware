#include "base.h"

namespace AyPlayer {


void Base::buttonClickHandlerTask (  void* obj  ) {
	(void)obj;
	/*
	EC_BUTTON_NAME		b;
	uint8_t				qData;
	int					r;
	Base* o =( Base* ) obj;
	( void )r;
	while( true ) {
		USER_OS_QUEUE_RESET( o->cfg->os->qAyButton );  // Старые команды нас не интересуют.
		USER_OS_QUEUE_RECEIVE( o->cfg->os->qAyButton, &qData, portMAX_DELAY );

		b	=	static_cast< EC_BUTTON_NAME >( qData );


		 * Основное окно.

		if ( o->wNow == AYPLAYER_WINDOW_NOW::MAIN ) {
			if ( b == EC_BUTTON_NAME::ENTER_CLICK ) {
				switch( static_cast< uint32_t >( o->playState ) ) {
				case static_cast< uint32_t >( AYPLAYER_STATUS::STOP ):
					USER_OS_GIVE_BIN_SEMAPHORE( o->cfg->os->sStartPlay );
					break;

				case static_cast< uint32_t >( AYPLAYER_STATUS::PAUSE ):
					o->playPauseSet( true );
					//o->guiUpdate();
					continue;

				case static_cast< uint32_t >( AYPLAYER_STATUS::PLAY ):
					o->playPauseSet( false );
					//o->guiUpdate();
					continue;
				};
			}

			if ( b == EC_BUTTON_NAME::ENTER_LONG_PRESS ) {
				if ( ( o->playState == AYPLAYER_STATUS::PLAY ) || ( o->playState == AYPLAYER_STATUS::PAUSE ) ) {
					o->stopPlayFile();
					//o->guiUpdate();
				}
				continue;
			}

			if ( b == EC_BUTTON_NAME::LEFT_CLICK ) {
				if ( o->currentFile > 0 ) {
					o->currentFile--;
					o->stopPlayFile();
					o->startPlayTrack();
					mSlimHorizontalListLeft( &o->g.shl, o->fat.currentFileInfo.fileName );
					//o->guiUpdate();
				}
				continue;
			}

			if ( b == EC_BUTTON_NAME::RIGHT_CLICK ) {
				if ( o->currentFile < o->countFileInCurrentDir - 1 ) {
					o->currentFile++;
					o->stopPlayFile();
					o->startPlayTrack();
					mSlimHorizontalListRight( &o->g.shl, o->fat.currentFileInfo.fileName );
					//o->guiUpdate();
				}
				continue;
			}

			/// Переход в эквалайзер.
			if ( b == EC_BUTTON_NAME::DOWN_CLICK ) {
				o->removePlayWindow();									/// Закрываем текущее окно.
				o->wNow = AYPLAYER_WINDOW_NOW::EQUALIZER;				/// Говорим что следующее будет эквалайзер.
				o->initEqualizerWindow();
				//o->guiUpdate();
				continue;
			}
		}


		 * Окно эквалайзера.

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
		}
	}
	*/
}

}

