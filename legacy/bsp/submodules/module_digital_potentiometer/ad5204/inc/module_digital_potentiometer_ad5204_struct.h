#pragma once

#ifdef MODULE_DIGITAL_POTENTIOMETER_ENABLED

#include "user_os.h"
#include "mc_spi_master_8bit.h"
#include "mc_pin.h"

namespace DigitalPotentiometer {

/// Смотрим, сколько целых байт занимает буфер +
/// если хоть 1 бит еще нужен сверху целого количества, добавляем 1 байт.
/// +1 байт еще для "разбега" алгоритма. Чтобы не делать лишних проверок
/// на выход за пределы массива
#define AD5204_BUF_SIZE(COUNT)				( ( COUNT * 11 ) / 8 ) + ( ( ( ( COUNT * 11 ) % 8) != 0 ) ? 1 : 0 ) + 1

/// Внутренняя структура буфер, которая хранит текущие
/// значения внутри потенциометра (одной микросхемы).
struct AD5204chipData {
    uint8_t				regData[4];
};

struct Ad5204StaticCfg {
    mc_interfaces::SpiMaster8Bit*		const spi;
    USER_OS_STATIC_MUTEX*						const mutex;		/// Для предотвращения коллизия на шине SPI.
    mc_interfaces::Pin*					const cs;			/// Выбор чипа(-ов). Активный 0.
    mc_interfaces::Pin*					const shdn;			/// Прямое включение (см. документацию).
    const uint32_t								countChip;			/// Количество потенциометров, включенных последовательно.
    uint8_t*									bufOutput;			/// Массив для внутренних нужд
                                                                    /// (размер получать макросом AD5204_BUF_SIZE).
    uint32_t									arraySize;			/// AD5204_BUF_SIZE.
    DigitalPotentiometer::AD5204chipData*		internalStructData;	/// Количество структур равно countChip.
};

}

#endif
