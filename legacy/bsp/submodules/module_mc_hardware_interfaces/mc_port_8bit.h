/*

@startuml

namespace mc_interfaces {

interface Port8bit {
	{abstract}{method}+	void		write		( uint8_t	data )
	{abstract}{method}+	void		reset		( void )
	{abstract}{method}+	void		toggle	( void )
	{abstract}{method}+	uint8_t	read		( void )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include <stdint.h>

namespace mc_interfaces {

/*!
 * Класс предназначен для управления одной линией ввода/вывода.
 * Вывод должен быть заранее настроен на выход или вход с нужными
 * параметрами.
 */
class Port8bit {
public:
    /// Записывает данные в порт.
    virtual void write (uint8_t data) = 0;
    
    /// Сбрасывает все биты в 0.
    virtual void reset (void) = 0;
    
    /// Инвертирует все биты.
    virtual void toggle (void) = 0;
    
    /// Возвращает состояние всех выставленных бит.
    /// От реализации зависит, возвращается реальное состояние или
    /// выставленное.
    virtual uint8_t read (void) = 0;
    
};

}

#endif
