/*

@startuml

namespace mc_interfaces {

interface Rcc {
	{abstract}{method}+	RccResult	setCfg		( uint32_t		cfgNumber = 0 )
	{abstract}{method}+	RccResult	getCfgNumber	( uint32_t&		returnCfgNumber )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include <stdint.h>

namespace mc_interfaces {

/// Варианты ответа на вызов метода setCfg класса RccBase.
enum class RccResult {
    ok = 0,            /// Операция успешна произведена.
    errCfgNumber = 1,            /// Попытка установить неподдерживаемый режим работы.
    errClkInit = 2,            /// Ошибка при настройке частоты.
    errOscInit = 3,            /// Ошибка при настройке источника тактового сигнала.
    errNoInit = 4                /// Попытка запросить конфигурацию у ни разу не
    /// устанавливающего конфигурацию модуля или провалившего
    /// предыдущую инициализацию.
};

/*!
 * Класс предназначен для управления частотой
 * тактового сигнала аппаратных блоков
 * микроконтроллера.
 */
class clk {
public:
    /*!
     * Сбрасывает текущие настройки RCC и инициализирует его заново
     * согласно выбранному режиму.
     *
     * \param[in]	numberCfg		-	выбранный режим тактирования.
     *
     * \return		{Итог попытки смены режим работы RCC.}
     */
    virtual RccResult setCfg (uint32_t cfgNumber = 0) = 0;
    
    virtual RccResult getCfgNumber (uint32_t &returnCfgNumber) = 0;
};

}

#endif
