#include "ayplayer_rcc.h"
#include "ayplayer_nvic.h"

namespace AyPlayer {

Rcc::Rcc ( const AyPlayerMcuCfg*	const mcuCfg ) : mcuCfg( mcuCfg ) {}

McHardwareInterfaces::RccResult Rcc::setCfg ( uint32_t cfgNumber ) {
	__disable_irq();

	this->mcuCfg->wdt->resetService();
	this->offObjDependingRcc();

	McHardwareInterfaces::RccResult r;
	r = this->mcuCfg->rcc->setCfg( cfgNumber );

	if ( r == McHardwareInterfaces::RccResult::ok ) {
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
	McHardwareInterfaces::BaseResult	r;

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

void Rcc::checkAndExit ( McHardwareInterfaces::BaseResult resultValue ) {
	if ( resultValue != McHardwareInterfaces::BaseResult::ok ) {
		AyPlayer::Nvic::reboot();
	}
}

void Rcc::startBaseInterfaces ( void ) {
	McHardwareInterfaces::BaseResult	r;

	r = this->mcuCfg->adcBat->startContinuousConversion();
	this->checkAndExit( r );

	r = this->mcuCfg->debugUart->on();
	this->checkAndExit( r );

	r = this->mcuCfg->spiBoard->on();
	this->checkAndExit( r );

	r = this->mcuCfg->spiLcd->on();
	this->checkAndExit( r );
}

McHardwareInterfaces::BaseResult Rcc::initRccFrequancyMax ( void ) {
	if ( this->setCfg( RCC_SPEED_FREQ_VERY_HIGH ) == McHardwareInterfaces::RccResult::ok ) {
		return McHardwareInterfaces::BaseResult::ok;
	} else {
		return McHardwareInterfaces::BaseResult::errInit;
	}
}

}
