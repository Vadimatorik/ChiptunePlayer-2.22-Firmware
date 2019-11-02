#include "base.h"

namespace AyPlayer {

uint32_t Base::getStatePlay ( void ) {
	return static_cast< uint32_t >( this->playState );
}

uint32_t Base::getPercentBattery ( void ) {
	return 0;
}

}
