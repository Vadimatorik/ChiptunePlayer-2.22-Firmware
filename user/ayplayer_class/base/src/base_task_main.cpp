#include "base.h"

namespace AyPlayer {

/// Для отслеживания клавиш качельки громкости.
#define INC_AND_DEC_STABIL_TIME  			    	100
#define OFF_WAIT_TIME             				   	3000
#define PERIOD_DETECT_PRESS_BUTTON_INC_DEC			10


/*!
 * Основная задача в начале производит
 * инициализацию устройства по деревьям, после чего
 * ждет разряда аккумулятора или выключения кнопками.
 */
void Base::mainTask ( void* obj ) {
	Base* o =( Base* ) obj;

	if ( o->printMessage( RTL_TYPE_M::RUN_MESSAGE_ISSUE, "System was been reboot!" ) != EOK )		abort();

	o->sd->init();
	o->gui->init();
	o->cfg->muxer->init();

	o->setSyscfgDefaultGuiParams();

	do {
		o->waitToInsertCorrectSdCard();

		//bool systemOsFileFlag;
		//if ( o->isSystemOsFile( systemOsFileFlag )		!= EOK )				continue;
		//if ( systemOsFileFlag == true ) {
		//
		//}
		if ( o->initFileLists()		!= EOK )				continue;
		if ( o->setSysCfgParams() 	!= EOK )				continue;
		if ( o->initMainWindow()	!= EOK )				continue;
	} while( false );

	/// Счетчики отсчета времени.
	int32_t incStabilTime	= INC_AND_DEC_STABIL_TIME;
	int32_t decStabilTime	= INC_AND_DEC_STABIL_TIME;
	int32_t offTime			= OFF_WAIT_TIME;

	/// Флаги факта нажатия.
	bool incF = false;
	bool decF = false;

	while ( true ) {
		USER_OS_DELAY_MS( PERIOD_DETECT_PRESS_BUTTON_INC_DEC );

		/// Состояние клавиш.
		incF = o->cfg->mcu->gpio->vr.inc->read();
		decF = o->cfg->mcu->gpio->vr.dec->read();

		/// Обе зажатые клавиши - старт отсчета времени нажатия для отключения.
		if ( ( incF == false ) && ( decF == false ) ) {
			if ( offTime > 0 ) {
				offTime -= PERIOD_DETECT_PRESS_BUTTON_INC_DEC;
			} else {
				o->powerOff( CAUSE_SHUTDOWN::USER_OFF );
			}
		} else {
			offTime = OFF_WAIT_TIME;
		}

		/// +.
		if ( incF == false ) {
			if ( incStabilTime > 0 ) incStabilTime -= PERIOD_DETECT_PRESS_BUTTON_INC_DEC;
		} else {
			if ( incStabilTime == 0 ) {
				if ( o->currentVolumeIndex < 15 )
					o->currentVolumeIndex++;
				incStabilTime = INC_AND_DEC_STABIL_TIME;

			/*
				 * Сопротивление потенциометров меняем только если идет воспроизвдеение или
				 * пауза, поскольку при старне трека с начала значение потенциометров нарастает плавно во
				 * избежание стука.
*/


					o->cfg->muxer->setVolume(	o->volumeTable[ o->currentVolumeIndex ]	);
					o->gui->update();
					continue;
				}
			}

		/// -.
		if ( decF == false ) {
			if ( decStabilTime > 0 ) decStabilTime -= PERIOD_DETECT_PRESS_BUTTON_INC_DEC;
		} else {
			if ( decStabilTime == 0 ) {
				if ( o->currentVolumeIndex > 0 )
					o->currentVolumeIndex--;
				decStabilTime = INC_AND_DEC_STABIL_TIME;

				o->cfg->muxer->setVolume(	o->volumeTable[ o->currentVolumeIndex ]	);
				o->gui->update();

				continue;
			}
		}



	}






	while( 1 ) {
		vTaskDelay( 1000 );
	}
}

}
