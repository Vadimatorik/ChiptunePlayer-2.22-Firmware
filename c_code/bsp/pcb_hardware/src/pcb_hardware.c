#include "mc_hardware.h"

#include "aym_hardware.h"

#include "dp.h"
#include "sr.h"
#include "ltc6903.h"
#include "matrix_keyboard.h"

int init_pcb_hardware () {
    int rv = 0;

    if ((rv = init_dp()) != 0) {
        return rv;
    }

    if ((rv = init_sr()) != 0) {
        return rv;
    }

    if ((rv = init_aym_hardware()) != 0) {
        return rv;
    }

    if ((rv = init_ltc6903()) != 0) {
        return rv;
    }

    if ((rv = init_matrix_keyboard()) != 0) {
        return rv;
    }
    
    return rv;
}