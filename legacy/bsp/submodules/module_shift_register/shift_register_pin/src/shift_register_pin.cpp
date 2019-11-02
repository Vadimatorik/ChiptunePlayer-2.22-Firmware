#include "shift_register_pin.h"

namespace ShiftRegister {

void Pin::set (void) {
    uint8_t buf;
    this->cfg->sr->readByte(this->cfg->byte, &buf);
    buf |= 1 << this->cfg->bit;
    this->cfg->sr->writeByte(this->cfg->byte, buf);
    this->cfg->sr->update();
}

void Pin::reset (void) {
    uint8_t buf;
    this->cfg->sr->readByte(this->cfg->byte, &buf);
    buf &= ~((uint8_t)(1 << this->cfg->bit));
    this->cfg->sr->writeByte(this->cfg->byte, buf);
    this->cfg->sr->update();
}

void Pin::toggle (void) {
    this->set(!this->read());
}

void Pin::set (bool state) {
    if (state) {
        this->set();
    } else {
        this->reset();
    }
}

void Pin::set (int state) {
    this->set(static_cast< bool >( state ));
}

void Pin::set (uint8_t state) {
    this->set(static_cast< bool >( state ));
}

bool Pin::read (void) {
    uint8_t buf;
    this->cfg->sr->readByte(this->cfg->byte, &buf);
    buf &= 1 << this->cfg->bit;
    
    return static_cast< bool >( buf );
}

}
