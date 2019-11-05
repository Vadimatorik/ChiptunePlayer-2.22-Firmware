#include "ayplayer_pcb_hardware.h"
#include "shift_register_8bit_port.h"


void Base::init ( void ) {
    this->st->set( !this->strobActive );					// Переводим защелку в неактивное положение.
}

int Base::readByte (	uint32_t	byteNumber,
                        uint8_t*	returnByte	) {
    if ( byteNumber >= this->arraySize )
        return -1;

    *returnByte = this->dataArray[ byteNumber ];

    return 0;
}

int Base::writeByte (	uint32_t	byteNumber,
                         uint8_t		writeData	) {
    if ( byteNumber >= this->arraySize )
        return -1;

    this->dataArray[ byteNumber ] = writeData;

    return 0;
}
// Перезаписать все на выходах.
void Base::update ( void ) {
    if ( this->mutex != nullptr )
        USER_OS_TAKE_MUTEX( *this->mutex, portMAX_DELAY );

    this->spiObj->tx( this->dataArray, this->arraySize, 10 );

    // Защелкой делаем обновление.
    this->st->set( this->strobActive );
    this->st->set( !this->strobActive );

    if ( this->mutex != nullptr)
        USER_OS_GIVE_MUTEX( *this->mutex );					// Разрешаем использование SPI другим потокам.
}

const ShiftRegister::Port8bitCfg chip1Port = {
    .byte    =    2,
    .sr        =    &boardShiftRegisters
};

ShiftRegister::Port8bit chip1(&chip1Port);

const ShiftRegister::Port8bitCfg chip2Port = {
    .byte    =    1,
    .sr        =    &boardShiftRegisters
};

ShiftRegister::Port8bit chip2(&chip2Port);

mc_interfaces::Port8bit *ayPorts[2] = {
    &chip1,
    &chip2
};
