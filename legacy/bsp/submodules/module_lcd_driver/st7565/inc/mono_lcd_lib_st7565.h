#pragma once

#include "project_config.h"

#ifdef MODULE_MONO_LCD_ST7565_ENABLED

#include "lcd_interface.h"
#include "mc_pin.h"
#include "mc_spi_master_8bit.h"
#include "user_os.h"

namespace mono_lcd {

enum class st7565_mode {
    standard = 0,
    ivert_x = 1,
    ivert_y = 2,
    ivert_x_amd_y = 3
};

//
// Парметры SPI MOTOROLA, CPHA = 1, CPOL = 1.
//
// Структура конфигурации ssd1306 в 8-ми битнос SPI режиме.
struct st7565_cfg {
    mc_interfaces::Pin *const a0;
    mc_interfaces::Pin *const res;
    mc_interfaces::Pin *const cs;
    mc_interfaces::SpiMaster8Bit *const s;
    const st7565_mode mode;
};

#define ST7565_WIDTH 128
#define ST7565_HEIGHT 64

/*
 * Любой из методов класса долен быть вызван только
 * внутри потока пользовательской операционной системы.
 */
class st7565 : public mono_lcd::base {
public:
    
    st7565 (const st7565_cfg *const cfg,
            uint8_t *const userBuf);
    
    ~st7565() = default;
    
    mc_interfaces::res reset (void);
    mc_interfaces::res set_contrast (uint8_t val);
    mc_interfaces::res on (void);
    mc_interfaces::res off (void);
    mc_interfaces::res update (void);
    mc_interfaces::res lcd_clear (void);
    void buf_clear (void);

private:
    mc_interfaces::res comOut (uint8_t command);
    mc_interfaces::res dataOut (uint8_t data);

private:
    bool flagInitLcd = false;        // Инициализирован ли LCD?
    const st7565_cfg *const cfg;
    uint8_t *const userBuf;        // Сам буффер менять можно, но указетль на него - нет.
    // Системный буфер для преобразования.
    // Дисплей едресуется вертикальными столбцами по 8 бит в высоту и 1 ширину. И так 8 строк.
    uint8_t lcdImage[128] = {0};
    // Для предотвращения попытки использовать LCD из разных потоков одновременно.
    StaticSemaphore_t mb;
    SemaphoreHandle_t m = nullptr;
};

}

#endif
