#include "shift_register_base.h"

#ifdef MODULE_SHIFT_REGISTER_ENABLED

namespace ShiftRegister {

Base::Base ( const BaseStaticCfg* const cfg ) :
    st( cfg->st ),
    dataArray( cfg->dataArray ),
    arraySize( cfg->arraySize ),
    strobActive( cfg->strobActive ),
    spiObj( cfg->spiObj ),
    mutex( cfg->mutex ) {}

Base::Base ( const BaseDinamicCfg* const cfg ) :
    st( cfg->st ),
    arraySize( cfg->byteCount ),
    strobActive( cfg->strobActive ),
    spiObj( cfg->spiObj ),
    mutex( cfg->mutex ) {
    dataArray = new uint8_t[ cfg->byteCount ];
}


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

}

#endif
