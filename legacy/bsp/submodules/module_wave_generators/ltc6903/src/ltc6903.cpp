#include "ltc6903.h"

namespace WaveGenerators {

LTC6903::LTC6903 (const LTC6903Cfg *const cfg) : cfg(cfg) {
    this->m = USER_OS_STATIC_MUTEX_CREATE(&this->mb);
}

int LTC6903::init (void) {
    USER_OS_TAKE_MUTEX(this->m, portMAX_DELAY);
    
    this->cfg->cs->set();
    this->cfg->cs->reset();
    uint16_t outputBuf = 3;
    uint8_t arrayOut[2];
    arrayOut[0] = outputBuf >> 8;
    arrayOut[1] = outputBuf & 0xFF;
    
    mc_interfaces::res r = this->cfg->s->tx(arrayOut, 2, 10);
    this->cfg->cs->set();
    
    USER_OS_GIVE_MUTEX(this->m);
    
    if (r == mc_interfaces::res::err_ok) {
        return EOK;
    } else {
        return EIO;
    }
}

int LTC6903::setFrequency (float frequency,
                           LTC6903OutputMode mode) {
    uint32_t oct = 0;
    
    if ((frequency < 1.039 * 10e3) || (frequency > 68.03 * 10e6)) {
        return EINVAL;
    }
    
    USER_OS_TAKE_MUTEX(this->m, portMAX_DELAY);
    
    if (frequency < 2.076e3) {
        oct = 0;
    } else if (frequency < 4.152e3) {
        oct = 1;
    } else if (frequency < 8.304e3) {
        oct = 2;
    } else if (frequency < 16.61e3) {
        oct = 3;
    } else if (frequency < 33.22e3) {
        oct = 4;
    } else if (frequency < 66.43e3) {
        oct = 5;
    } else if (frequency < 132.9e3) {
        oct = 6;
    } else if (frequency < 265.7e3) {
        oct = 7;
    } else if (frequency < 531.4e3) {
        oct = 8;
    } else if (frequency < 1063e3) {
        oct = 9;
    } else if (frequency < 2.126e6) {
        oct = 10;
    } else if (frequency < 4.252e6) {
        oct = 11;
    } else if (frequency < 8.503e6) {
        oct = 12;
    } else if (frequency < 17.01e6) {
        oct = 13;
    } else if (frequency < 34.01e6) {
        oct = 14;
    } else if (frequency < 68.03e6) {
        oct = 15;
    }
    
    uint16_t dac;
    dac = 2048 - (2078 * pow(2, 10 + oct)) / frequency;
    
    uint16_t outputBuf = oct << 12;
    outputBuf |= dac << 2;
    
    outputBuf |= (uint8_t)mode;
    
    uint8_t arrayOut[2];
    arrayOut[0] = outputBuf >> 8;
    arrayOut[1] = outputBuf & 0xFF;
    
    this->cfg->cs->reset();
    mc_interfaces::res r = this->cfg->s->tx(arrayOut, 2, 10);
    this->cfg->cs->set();
    
    USER_OS_GIVE_MUTEX(this->m);
    
    if (r == mc_interfaces::res::err_ok) {
        return EOK;
    } else {
        return EIO;
    }
}


}
