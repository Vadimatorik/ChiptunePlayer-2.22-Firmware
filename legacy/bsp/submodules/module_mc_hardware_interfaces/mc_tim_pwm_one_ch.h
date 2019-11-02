/*

@startuml

namespace mc_interfaces {

interface TimPwmOneChannel {
	{abstract}{method}+	res		reinit		( uint32_t	cfgNumber = 0 )
	{abstract}{method}+	res		on		( void )
	{abstract}{method}+	void			off		( void )
	{abstract}{method}+	void			setDuty	( float	duty )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "mc_base.h"

namespace mc_interfaces {

/*!
 * Класс предназначен для генерации PWM сигнала на одном канале аппаратного таймера.
 */
class TimPwmOneChannel {
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
    
    /*!
     * Устанавливает коэффициент заполнения ШИМ-сигнала.
     * \param[in]	duty			-	кожффициент заполнения в диапазоне 0..1.
     */
    virtual void setDuty (float duty) = 0;
    
};

}

#endif
