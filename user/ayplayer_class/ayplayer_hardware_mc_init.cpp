#include "ayplayer.h"

namespace AyPlayer {

void AyPlayer::hardwareMcInit ( void ) {
	McHardwareInterfaces::BaseResult r;

	r = this->cfg->mcu->wdt->reinit();
	this->checkAndExit( r );

	r = this->cfg->mcu->pwr->reinit();
	this->checkAndExit( r );

	r = this->cfg->mcu->gp->reinitAllPorts();
	this->checkAndExit( r );

	r = this->rcc->initRccFrequancyMax();
	this->checkAndExit( r );

	this->nvic.setPriorityCallsInterruptVectors();
	this->nvic.enableAllIrq();
}

}
