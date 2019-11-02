/*

@startuml

namespace mc_interfaces {

interface AdcOneChannel {
	{abstract}{method}+	res		reinit						( uint32_t cfgNumber = 0 )
	{abstract}{method}+	res		startContinuousConversion		( void )
	{abstract}{method}+	void			stopContinuousConversion		( void )
	{abstract}{method}+	uint32_t		getMeasurement				( void )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "mc_base.h"

namespace mc_interfaces {

/*!
 * Класс предназначен для использования одного канала ADC
 * в режиме непрерывного преобразования.
 */
class AdcOneChannel {
public:
    /*!
     * Сбрасывает текущие настройки ADC и инициализирует его заново.
     * После инициализации ADC остается отключенным.
     *
     * Замечание: управление тактовым сигналом аппаратного
     * модуля осуществляется внутри метода автоматически.
     *
     * \param[in]	numberCfg		-	выбранный режим работы.
     *
     * 	\return		{	BASE_RESULT::OK			-	инициализация прошла успешно.
     *					BASE_RESULT::INPUT_VALUE_ERROR	-	несуществующий номер конфигурации.
     *					BASE_RESULT::ERROR_INIT	-	в противном случае.	}
     */
    virtual res reinit (uint32_t numberCfg = 0) = 0;
    
    /*!
     * Инициирует непрерывное преобразование на канале.
     *
     * \return		{	BASE_RESULT::OK			-	последовательность преобразований
     *												была успешно начата.
     *					BASE_RESULT::ERROR_INIT	-	в противном случае.	}
     */
    virtual res startContinuousConversion (void) = 0;
    
    /// Останавливает непрерывное преобразование на канале.
    virtual void stopContinuousConversion (void) = 0;
    
    /// Возвращает результат измерения.
    /// \return		{	Результат измерения напряжения на канале.	}
    virtual uint32_t getMeasurement (void) = 0;
    
};

}

#endif
