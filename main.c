#include "freertos_headers.h"
#include "hardware.h"
#include "lua.h"

int main () {
    int rv = 0;

    if ((rv = init_core()) != 0) {
        return rv;
    }

    if ((rv = init_gpio()) != 0) {
        return rv;
    }

    if ((rv = init_rcc()) != 0) {
        return rv;
    }

    vTaskStartScheduler();
}
