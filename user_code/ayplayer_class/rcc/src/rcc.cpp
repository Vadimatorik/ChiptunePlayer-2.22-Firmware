#include "rcc.h"

#include "nvic.h"

namespace AyPlayer {

Rcc::Rcc ( const AyPlayerMcuCfg*	const mcuCfg ) : mcuCfg( mcuCfg ) {}

mc_interfaces::RccResult Rcc::setCfg ( uint32_t cfgNumber ) {
	__disable_irq();

	this->mcuCfg->wdt->resetService();
	this->offObjDependingRcc();

	mc_interfaces::RccResult r;
	r = this->mcuCfg->rcc->setCfg( cfgNumber );

	if ( r == mc_interfaces::RccResult::ok ) {
		this->rccIndex = cfgNumber;
		reinitObjDependingRcc();
		startBaseInterfaces();
		SysTick_Config( SystemCoreClock / 1000 );
	}

	__enable_irq();

	return r;
}

void Rcc::offObjDependingRcc ( void ) {
	this->mcuCfg->lcdPwmTim->off();
	this->mcuCfg->interruptAyTim->off();

#ifdef configGENERATE_RUN_TIME_STATS
	this->mcuCfg->timRunTimeStatus->off();
#endif

	this->mcuCfg->adcBat->stopContinuousConversion();

	this->mcuCfg->spiBoard->off();
	this->mcuCfg->spiLcd->off();

	this->mcuCfg->debugUart->off();
}

void Rcc::reinitObjDependingRcc ( void ) {
	mc_interfaces::res	r;

	r = this->mcuCfg->lcdPwmTim->reinit( this->rccIndex );
	this->checkAndExit( r );
	r = this->mcuCfg->interruptAyTim->reinit( this->rccIndex );
	this->checkAndExit( r );

#ifdef configGENERATE_RUN_TIME_STATS
	r = this->mcuCfg->timRunTimeStatus->reinit( this->rccIndex );
	this->checkAndExit( r );
	r = this->mcuCfg->timRunTimeStatus->on();
	this->checkAndExit( r );
#endif

	r = this->mcuCfg->adcBat->reinit( this->rccIndex );
	this->checkAndExit( r );

	r = this->mcuCfg->spiBoard->reinit();
	this->checkAndExit( r );
	r = this->mcuCfg->spiBoard->setPrescaler( this->rccIndex );
	this->checkAndExit( r );

	r = this->mcuCfg->spiLcd->reinit();
	this->checkAndExit( r );
	r = this->mcuCfg->spiLcd->setPrescaler( this->rccIndex );
	this->checkAndExit( r );

	r = this->mcuCfg->debugUart->reinit();
	this->checkAndExit( r );
}

void Rcc::checkAndExit ( mc_interfaces::res resultValue ) {
	if ( resultValue != mc_interfaces::res::err_ok ) {
		AyPlayer::Nvic::reboot();
	}
}

void Rcc::startBaseInterfaces ( void ) {
	mc_interfaces::res	r;

	r = this->mcuCfg->adcBat->startContinuousConversion();
	this->checkAndExit( r );

	r = this->mcuCfg->debugUart->on();
	this->checkAndExit( r );

	r = this->mcuCfg->spiBoard->on();
	this->checkAndExit( r );

	r = this->mcuCfg->spiLcd->on();
	this->checkAndExit( r );
}

mc_interfaces::res Rcc::initRccFrequancyMax ( void ) {
	if ( this->setCfg( RCC_SPEED_FREQ_VERY_HIGH ) == mc_interfaces::RccResult::ok ) {
		return mc_interfaces::res::err_ok;
	} else {
		return mc_interfaces::res::err_init;
	}
}

}
