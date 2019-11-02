/*

@startuml

namespace mc_interfaces {

interface Uart {
	{abstract}{method}+	res		reinit				( uint32_t			cfgNumber		= 0 )
	{abstract}{method}+	res		on				( void )
	{abstract}{method}+	void			off				( void )
	{abstract}{method}+	res		tx				( const uint8_t*		const txArray,\n\t\t\t\t\t\t\t  uint16_t			length		= 1,\n\t\t\t\t\t\t\t  uint32_t			timeoutMs		= 100 )
	{abstract}{method}+	res		getByte			( uint8_t*			retrunData,\n\t\t\t\t\t\t\t  uint32_t			timeoutMs		= 100 )
	{abstract}{method}+	res		getByteWitchout		( uint8_t* 			retrunData )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "mc_base.h"

namespace mc_interfaces {

/*!
 * Класс предназначен для приема/передачи через аппаратный uart/usart.
 */
class uart {
public:
    /*!
     * Сбрасывает текущие настройки UART/USART и инициализирует его заново.
     * После инициализации UART/USART остается отключенным.
     *
     * Замечание: управление тактовым сигналом аппаратного
     * модуля осуществляется внутри метода автоматически.
     *
     * \param[in]	cfgNumber		-	выбранный режим работы.
     *
     * \return		{	BASE_RESULT::OK					-	инициализация прошла успешно.
     *					BASE_RESULT::INPUT_VALUE_ERROR	-	несуществующий номер конфигурации.
     *					BASE_RESULT::ERROR_INIT			-	ошибка инициализации.
     */
    virtual res reinit (uint32_t cfgNumber = 0) = 0;
    
    /*!
     * Запускает UART/USART.
     *
     * \return		{	BASE_RESULT::OK					-	передача прошла успешно.
     *					BASE_RESULT::ERROR_INIT			-	UART/USART не был инициализирован ранее.	}
     */
    virtual res on (void) = 0;
    
    /// Останавливает UART/USART.
    virtual void off (void) = 0;
    
    /*!
     * Передает массив по uart/usart игнорируя приходящие байты.
     *
     * \param[in]	txArray		-	указатель на массив, который требуется передать
     *								по uart/usart. Указатель на 0-й передаваемый байт
     *								( байт, с которого будет начата передача ).
     * \param[in]	length		-	длина посылки.
     *								Условие; length != 0 и length >= 1!
     * \param[in]	timeoutMs	-	если посылка не была передана в течении этого времени
     *								( в миллисекундах ), то будет
     *								возвращена соответствующая ошибка.
     * \param[in]	stepMode	-	направление движения по массиву.
     *
     * \return		{	BASE_RESULT::OK					-	передача прошла успешно.
     *					BASE_RESULT::ERROR_INIT			-	UART/USART не был инициализирован ранее.
     *					BASE_RESULT::LENGTH_ERROR		-	length вне диапазона.
     *					BASE_RESULT::INPUT_VALUE_ERROR	-	txArray == nullptr.
     *					BASE_RESULT::TIME_OUT			-	время ожидания истекло.	}
     */
    virtual res tx (const uint8_t *const txArray,
                           uint16_t length = 1,
                           uint32_t timeoutMs = 100) = 0;
    
    /*!
     * Возвращает пришедший по UART/USART байт, если таковой имеется.
     *
     * \param[out]	retrunData	-	принятый байт.

     * \return		{	BASE_RESULT::OK					-	байт принят успешно.
     *					BASE_RESULT::ERROR_INIT			-	UART/USART не был инициализирован ранее.
     *					BASE_RESULT::TIME_OUT			-	время ожидания истекло.	}
     */
    virtual res get_byte (uint8_t *retrunData,
                                uint32_t timeoutMs = 100) = 0;
    
    /*!
     * Возвращает пришедший по UART/USART байт без ожидания.
     *
     * \param[out]	retrunData	-	принятый байт.

     * \return		{	BASE_RESULT::OK					-	байт принят успешно.
     *					BASE_RESULT::ERROR_INIT			-	UART/USART не был инициализирован ранее.
     *					BASE_RESULT::NOT_DATA			-	байт отсутствует	}
     */
    virtual res get_byte (uint8_t *retrunData) = 0;
    
};

}

#endif
