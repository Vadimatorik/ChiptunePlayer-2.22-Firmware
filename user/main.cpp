#include "base.h"

///extern const AyPlayer::BaseCfg ayCfg;
///AyPlayer::Base ay( &ayCfg );

extern "C" {

int main (void) {
   // ay.start();
}

volatile uint32_t i = 0;

void Reset_Handler (void) {
    ///main();
    while(1) {
        i++;
    }
}

}