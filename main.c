#include "freertos_headers.h"
#include "hardware.h"

int main () {
    int rv = 0;
    if ((rv = init_rcc_hse_16_mhz()) != 0) {
        return rv;
    }

    vTaskStartScheduler();
}
