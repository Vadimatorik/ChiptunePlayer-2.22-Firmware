/*

@startuml

namespace MonoLcd {

interface Base {
	{abstract}{method}+	mc_interfaces::res	reset		( void )
	{abstract}{method}+	mc_interfaces::res	on		( void )
	{abstract}{method}+	mc_interfaces::res	off		( void )
	{abstract}{method}+	mc_interfaces::res 	update	( void )
	{abstract}{method}+	mc_interfaces::res 	lcdClear	( void )
	{abstract}{method}+	void							bufClear	( void )
}

}

@enduml

*/
#pragma once

#include "mc_base.h"

namespace mono_lcd {

/*
 * Этот класс является чисто виртуальным базовым классом
 * для всех монохромных LCD с расширением 128x64.
 */
class base {
public:
	constexpr base() {}
	/*
	 * Метод должен сбрасывать LCD в начальное состояние (но не включать!).
	 */

	virtual mc_interfaces::res	reset			(	void	) = 0;

   /*
	* Включить/выключить дисплей.
	*/
	virtual mc_interfaces::res	on				(	void	) = 0;
	virtual mc_interfaces::res	off				(	void	) = 0;


	/*
	 * Вывод данных из буфера в сам LCD.
	 */
	virtual mc_interfaces::res 	update			(	void	) = 0;


	/*
	 * Очистка непосредственно экрана.
	 */
	virtual mc_interfaces::res 	lcd_clear (void) = 0;

	/*
	 * Очистка буфера экрана.
	 */
	virtual void								buf_clear (void) = 0;

	virtual ~base() {}
};

}

