**********************************************************************
// Данный поток будет выдавать из очереди 50 раз
// в секунду данные (данные разделяются 0xFF).
// Разблокируется из ay_timer_handler.
//**********************************************************************

void AyYmLowLavel::task ( void* p_this ) {

}

// Останавливаем/продолжаем с того же места воспроизведение. Синхронно для всех AY/YM.
void AyYmLowLavel::playStateSet ( uint8_t state ) {
	this->cfg->funcFrequencyAyControl( state );
	this->cfg->funcTimInterruptTask( state );

	if ( state )
		this->sendBuffer();
}

#endif
