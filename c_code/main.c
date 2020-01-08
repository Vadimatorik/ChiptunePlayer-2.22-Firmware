#include "freertos_headers.h"
#include "mc_hardware.h"
#include "l.h"

#ifdef AYM_SOFT
#include "socket_emul_layer.h"
#endif

int main () {
    int rv = 0;

#ifdef AYM_SOFT
    if ((rv = init_sockets()) != 0) {
        return rv;
    }
#endif

    if ((rv = init_mc_hardware()) != 0) {
        return rv;
    }

    if ((rv = init_lua_main_thread()) != 0) {
        return rv;
    }

    vTaskStartScheduler();
}
