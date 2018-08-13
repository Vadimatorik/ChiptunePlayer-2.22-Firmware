#include "ayplayer.h"

extern const AyPlayerCfg ayCfg;
AyPlayer ay( &ayCfg );

extern "C" {

int main ( void ) {

	if ( ay.fsmStepFuncHardwareMcInit() != 0 )
			NVIC_SystemReset();

		if ( ay.fsmStepFuncFreeRtosthisInit() != 0 )
				NVIC_SystemReset();

	ay.start();
}

}

