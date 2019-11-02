#include "shift_register_8bit_port.h"

namespace ShiftRegister {

void Port8bit::write (uint8_t data) {
    this->cfg->sr->writeByte(this->cfg->byte, data);
    this->cfg->sr->update();
}

void Port8bit::reset (void) {
    this->cfg->sr->writeByte(this->cfg->byte, 0);
    this->cfg->sr->update();
}

void Port8bit::toggle (void) {
    uint8_t data;
    this->cfg->sr->readByte(this->cfg->byte, &data);
    data = ~data;
    this->cfg->sr->writeByte(this->cfg->byte, data);
    this->cfg->sr->update();
}

uint8_t Port8bit::read (void) {
    uint8_t data;
    this->cfg->sr->readByte(this->cfg->byte, &data);
    return data;
}

}
