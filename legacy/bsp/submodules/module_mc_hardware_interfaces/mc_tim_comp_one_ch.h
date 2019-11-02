/*

@startuml

namespace mc_interfaces {

interface TimCompOneChannel {
	{abstract}{method}+	res		reinit		( uint32_t	cfgNumber = 0 )
	{abstract}{method}+	res		on		( void )
	{abstract}{method}+	void			off		( void )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "mc_base.h"

namespace mc_interfaces {

/*!
 * Класс предназначен для для генерации прямоугольных
 * импульсов на одном канале средствами аппаратного таймера.
 */
class TimCompOneChannel {
public:
    /*!
     * Сбрасывает текущие настройки таймера и инициализирует его заново.
     * После инициализации таймер остается отключенным.
     *
     * Замечание: управление тактовым сигналом аппаратного
     * модуля осуществляется внутри метода автоматически.
     *
     * \param[in]	cfgNumber		-	выбранная конфигурация таймера.
     *
     * \return		{	BASE_RESULT::OK					-	инициализация прошла успешно.
     *					BASE_RESULT::INPUT_VALUE_ERROR	-	несуществующий номер конфигурации.
     *					BASE_RESULT::ERROR_INIT			-	ошибка инициализации.
     */
    virtual res reinit (uint32_t cfgNumber = 0) = 0;
    
    /*!
     * Запускает таймера.
     *
     * \return		{	BASE_RESULT::OK					-	передача прошла успешно.
     *					BASE_RESULT::ERROR_INIT			-	SPI не был инициализирован ранее.	}
     */
    virtual res on (void) = 0;
    
    /// Останавливает таймер.
    virtual void off (void) = 0;
    
};

}

#endif
