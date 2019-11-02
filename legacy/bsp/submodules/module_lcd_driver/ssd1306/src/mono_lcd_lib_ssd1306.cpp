#include "mono_lcd_lib_ssd1306.h"

#ifdef MODULE_MONO_LCD_SSD1306_ENABLED

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// Массив значений-инициализаторов. Чтобы не засерать ОЗУ, сразу шлем из flash.
static const uint8_t ssd1306_init_command[] = {
    0xAE, 0x20, 0x10, 0xb0, 0xc8, 0x00, 0x10, 0x40,
    0x81, 0xff, 0xa1, 0xa6, 0xa8, 0x3F, 0xa4, 0xd3,
    0x00, 0xd5, 0xf0, 0xd9, 0x22, 0xda, 0x12, 0xdb,
    0x20, 0x8d, 0x14, 0xaf
};

Ssd1306::Ssd1306 ( const mono_lcd_lib_ssd1306_cfg_t* const cfg, uint8_t* const buf ) : cfg(cfg), buf(buf) {
    this->mutex     = USER_OS_STATIC_MUTEX_CREATE( &this->mutex_buf );
}

// Инициализируем LCD.
BASE_RESULT Ssd1306::reset ( void ) {
    cfg->dc->reset();                   // Далее идут комманды.
    cfg->cs->set();                     // Переводим дисплей в невыбранное положение.
    // Перезагружаем дисплей.
    cfg->res->reset();
    USER_OS_DELAY_MS(10);
    cfg->res->set();
    USER_OS_DELAY_MS(10);               // Ждем, пока запустится.

    USER_OS_TAKE_MUTEX( this->mutex, portMAX_DELAY );   // Ждем, пока освободится SPI.

    cfg->cs->reset();  	// Выбираем наш дисплей.
    BASE_RESULT r;
    r = this->cfg->s->tx( ssd1306_init_command, sizeof( ssd1306_init_command ), 100 );
    cfg->cs->reset();
    USER_OS_GIVE_MUTEX( this->mutex );	// Разрешаем использование SPI другим потокам.

    checkResult( r );
    this->flag = true;
    return this->update();					// Обновляем буффер.
}

/*
 * Включать/выключать LCD.
 */
BASE_RESULT Ssd1306::on ( void ) {
    while( true ) {}; // Поднять потом.
}

BASE_RESULT Ssd1306::off ( void ) {
     while( true ) {}; // Поднять потом.
}

// Выбираем позицию на экране. Функция не следит за флагом SPI. CS так же не дергает.
BASE_RESULT Ssd1306::set_pos_to_lcd ( const uint8_t& x, const uint8_t& y ) {
    cfg->dc->reset(); // Далее идет комманда.
    uint8_t buffer_command[3];
    buffer_command[2] = 0xb0+y;
    buffer_command[1] = ( ( x & 0xf0 ) >> 4 ) | 0x10;
    buffer_command[0] = ( x & 0x0f ) | 0x01;
    BASE_RESULT r;
    r = this->cfg->s->tx( buffer_command, 3, 100 );
    return r;
}

// Выдаем данные из буфера.
BASE_RESULT Ssd1306::update ( void ) {
    BASE_RESULT r;
    if ( this->flag == 0 ) return BASE_RESULT::ERROR_INIT;		// Если экран еще не инициализирован - выходим.
    USER_OS_TAKE_MUTEX( this->mutex, portMAX_DELAY );   // Ждем, пока освободится SPI.

    cfg->cs->reset();               // Выбираем дисплей.

    r = this->set_pos_to_lcd( 0, 0 );

    if ( r == BASE_RESULT::OK )	{
        cfg->dc->set(); // Далее идут данные.

        r = this->cfg->s->tx( this->buf, 1024, 100 );

        cfg->cs->set();// Отсоединяем SPI от дисплея.
    }
    USER_OS_GIVE_MUTEX( this->mutex );	// Разрешаем использование SPI другим потокам.

    return r;
}

// Инициализируем LCD.
void Ssd1306::bufClear ( void ) {
    memset( this->buf, 0, 1024 );
}

#endif
