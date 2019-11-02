/*

@startuml

namespace mc_interfaces {

interface I2cMaster {
	{abstract}{method}+	res		reinit		( uint32_t		numberCfg = 0 )
	{abstract}{method}+	res		on		( void )
	{abstract}{method}+	void			off		( void )
	{abstract}{method}+	res		read		( uint8_t		slaveDeviceAddress,\n\t\t\t\t\t  uint8_t*		rx,\n\t\t\t\t\t  uint8_t		readAddress,\n\t\t\t\t\t  uint16_t		countByte )
	{abstract}{method}+	res		readDma	( uint8_t		slaveDeviceAddress,\n\t\t\t\t\t  uint8_t*		rx,\n\t\t\t\t\t  uint8_t		readAddress,\n\t\t\t\t\t  uint16_t		countByte )
	{abstract}{method}+	res		writeByte	( uint8_t		slaveDeviceAddress,\n\t\t\t\t\t  uint8_t*		rx,\n\t\t\t\t\t  uint8_t		writeAddress )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "mc_base.h"

namespace mc_interfaces {

/*!
 * Класс переназначен для работы с I2C
 * в режиме ведущего устройства на шине.
 */
class I2cMaster {
public:
    /*!
     * Сбрасывает текущие настройки I2C и инициализирует его заново.
     * После инициализации I2C остается отключенным.
     *
     * Замечание: управление тактовым сигналом аппаратного
     * модуля осуществляется внутри метода автоматически.
     *
     * \param[in]	numberCfg		-	выбранный режим работы.
     *
     * \return		{	BASE_RESULT::OK			-	инициализация прошла успешно.
     *					BASE_RESULT::INPUT_VALUE_ERROR	-	несуществующий номер конфигурации.
     *					BASE_RESULT::ERROR_INIT	-	в противном случае.	}
     */
    virtual res reinit (uint32_t numberCfg = 0) = 0;
    
    /*!
     * Запускает I2C.
     *
     * \return		{	BASE_RESULT::OK					-	передача прошла успешно.
     *					BASE_RESULT::ERROR_INIT			-	SPI не был инициализирован ранее.	}
     */
    virtual res on (void) = 0;
    
    /// Останавливает I2C.
    virtual void off (void) = 0;
    
    
    virtual res read (uint8_t slaveDeviceAddress,
                             uint8_t *rx,
                             uint8_t readAddress,
                             uint16_t countByte) = 0;
    
    virtual res readDma (uint8_t slaveDeviceAddress,
                                uint8_t *rx,
                                uint8_t readAddress,
                                uint16_t countByte) = 0;
    
    virtual res writeByte (uint8_t slaveDeviceAddress,
                                  uint8_t *rx,
                                  uint8_t writeAddress) = 0;
};

}

#endif
