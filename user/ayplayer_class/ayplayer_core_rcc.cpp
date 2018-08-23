#include "ayplayer.h"
#include "ayplayer_clock.h"

RccResult AyPlayer::setRccCfg ( uint32_t numberCfg ) {
	__disable_irq();

	this->cfg->mcu->wdt->resetService();
	this->offObjDependingRcc();

	RccResult r;
	r = this->cfg->mcu->rcc->setCfg( numberCfg );

	if ( r == RccResult::ok ) {
		this->rccIndex = numberCfg;
		reinitObjDependingRcc();
		SysTick_Config( SystemCoreClock / 1000 );
	}

	__enable_irq();
	return r;
}

void AyPlayer::offObjDependingRcc ( void ) {
	/// Таймеры.
	this->cfg->mcu->lcdPwmTim->off();
	this->cfg->mcu->interruptAyTim->off();

	/// Время в задачах.
#ifdef configGENERATE_RUN_TIME_STATS
	this->cfg->mcu->timRunTimeStatus->off();
#endif

	/// Аналого-цифровые преобразователи.
	this->cfg->mcu->adcBat->stopContinuousConversion();

	/// SPI.
	this->cfg->mcu->spiBoard->off();
	this->cfg->mcu->spiLcd->off();

	/// UART.
	this->cfg->mcu->debugUart->off();
}

void AyPlayer::reinitObjDependingRcc ( void ) {
	/// Таймеры.
	this->cfg->mcu->lcdPwmTim->reinit( this->rccIndex );
	this->cfg->mcu->interruptAyTim->reinit( this->rccIndex );

#ifdef configGENERATE_RUN_TIME_STATS
	this->cfg->mcu->timRunTimeStatus->reinit( this->rccIndex );
	this->cfg->mcu->timRunTimeStatus->on();
#endif

	/// Аналого-цифровые преобразователи.
	this->cfg->mcu->adcBat->reinit( this->rccIndex );

	/// SPI.
	this->cfg->mcu->spiBoard->reinit( 0 );
	this->cfg->mcu->spiBoard->setPrescaler( this->rccIndex );

	this->cfg->mcu->spiLcd->reinit( 0 );
	this->cfg->mcu->spiLcd->setPrescaler( this->rccIndex );

	/// UART всегда с одной скоростью.
	this->cfg->mcu->debugUart->reinit( 0 );
}

void AyPlayer::startBaseInterfaces ( void ) {
	BaseResult	r;
	r = this->cfg->mcu->adcBat->startContinuousConversion();
	assertParam( r == BaseResult::ok );
	r = this->cfg->mcu->debugUart->on();
	assertParam( r == BaseResult::ok );
	r = this->cfg->mcu->spiBoard->on();
	assertParam( r == BaseResult::ok );
	r = this->cfg->mcu->spiLcd->on();
	assertParam( r == BaseResult::ok );
}

int AyPlayer::rccMaxFrequancyInit ( void ) {
	if ( this->setRccCfg( RCC_SPEED_FREQ_VERY_HIGH ) != RccResult::ok )
		return -1;

	return 0;
}

uint32_t AyPlayer::getRccMode ( void ) {
	return this->rccIndex;
}
