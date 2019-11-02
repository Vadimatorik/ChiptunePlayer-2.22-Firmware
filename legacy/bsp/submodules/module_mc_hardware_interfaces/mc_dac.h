/*

@startuml

namespace mc_interfaces {

interface Dac {
	{abstract}{method}+	res		reinit		( uint32_t		numberCfg = 0 )
	{abstract}{method}+	res		setValue	( uint32_t		channel,\n\t\t\t\t\t  uint32_t		value )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "mc_base.h"

namespace mc_interfaces {

/*!
 * Класс предназначен для использования каналов DAC.
 */
class Dac {
public:
    /*!
     * Сбрасывает текущие настройки DAC и инициализирует его заново.
     * После инициализации на каналах DAC выставляются значения
     * по умолчанию.
     *
     * Замечание: управление тактовым сигналом аппаратного
     * модуля осуществляется внутри метода автоматически.
     *
     * \param[in]	numberCfg		-	выбранный режим работы.
     *
     * 	\return		{	BASE_RESULT::OK					-	инициализация прошла успешно.
     *					BASE_RESULT::INPUT_VALUE_ERROR	-	несуществующий номер конфигурации.
     *					BASE_RESULT::ERROR_INIT			-	в противном случае.	}
     */
    virtual res reinit (uint32_t cfgNumber = 0) = 0;
    
    /*!
     * Выставляет заданное значение на канал.
     * \param[in]	ch			-	канал, в который будет записано значение.
     * \param[in]	value		-	записываемое значение.
     *
     * \return		{	BASE_RESULT::OK					-	значение выставлено успешно.
     *					BASE_RESULT::ERROR_INIT			-	DAC не был инициализирован ранее.
     *					BASE_RESULT::INPUT_VALUE_ERROR	-	ch или value вне диапазона. }
     */
    virtual res setValue (uint32_t channel,
                                 uint32_t value) = 0;
};

}

#endif
