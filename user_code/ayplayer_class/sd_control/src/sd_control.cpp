#include "sd_control.h"

namespace AyPlayer {

SdControl::SdControl	(	mc_interfaces::PinMultifuncIt*	pinIt,
							mc_interfaces::Pin*				pinRead,
							mc_interfaces::Pin*				pinSet	)
								: pinIt( pinIt ), pinRead( pinRead ), pinSet( pinSet ) {
}

void SdControl::init ( void ) {
	this->pinSet->reset();
}

bool SdControl::getState ( void ) {
	return !this->pinRead->read();
}

}
