
/*struct PinCfg {
    const uint8_t byte;        /// Байт в сдвиговом регистре.
    const uint8_t bit;        /// Бит в сдвиговом регистре.
    ShiftRegister::Base *sr;            /// Сам сдвиговый регистр.
};

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



void sr_set_pin_ad5204_shdn () {

}

void sr_reset_pin_ad5204_shdn () {

}*/

