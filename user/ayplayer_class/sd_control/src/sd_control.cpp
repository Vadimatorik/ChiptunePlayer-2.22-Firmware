#include "../sd_control/sd_control.h"

namespace AyPlayer {

SdControl::SdControl	(	McHardwareInterfaces::PinMultifuncIt*	pinIt,
							McHardwareInterfaces::Pin*				pinRead,
							McHardwareInterfaces::Pin*				pinSet	)
								: pinIt( pinIt ), pinRead( pinRead ), pinSet( pinSet ) {
}

void SdControl::init ( void ) {
	this->pinSet->reset();
}

bool SdControl::getState ( void ) {
	return !this->pinRead->read();
}

}
