#pragma once

#ifdef MODULE_MONO_LCD_SSD1306_ENABLED

#include "lcd_interface.h"
#include "mc_pin.h"
#include "mc_spi_master_8bit.h"
#include "user_os.h"

//
// Парметры SPI MOTOROLA, CPHA = 1, CPOL = 1.
//
// Структура конфигурации ssd1306 в 8-ми битнос SPI режиме.
struct mono_lcd_lib_ssd1306_cfg_t {
    PinBase*					const cs;
    PinBase*					const res;
    PinBase*					const dc;
    SpiMaster8BitBase*			const s;
};

//
// Любой из методов класса долен быть вызван только
// внутри потока пользовательской операционной системы.
//
class Ssd1306 : public monoLcd128x64Base {
public:
    Ssd1306 ( const mono_lcd_lib_ssd1306_cfg_t* const cfg, uint8_t* const buf );

    BASE_RESULT	reset			( void );
    BASE_RESULT	update			( void ); 				// Обновляем LCD из буффера, в котором рисовали.
    BASE_RESULT	on				( void );
    BASE_RESULT	off				( void );

    void		bufClear		( void );				 // Очищаем буффер.

private:
    BASE_RESULT set_pos_to_lcd ( const uint8_t& x, const uint8_t& y );

    bool flag = false;	// Инициализирован ли LCD?
    const mono_lcd_lib_ssd1306_cfg_t* const cfg;
    uint8_t* const buf; // Сам буффер менять можно, но указетль на него - нет.
    // Для предотвращения попытки использовать LCD из разных потоков одновременно.
    StaticSemaphore_t	 mutex_buf;
    SemaphoreHandle_t			mutex = nullptr;
};

#endif
