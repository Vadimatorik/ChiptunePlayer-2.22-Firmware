#include "ayplayer.h"

extern const AyPlayerCfg ayCfg;
AyPlayer::AyPlayer ay( &ayCfg );

extern "C" {

int main ( void ) {
	ay.start();
}

}

