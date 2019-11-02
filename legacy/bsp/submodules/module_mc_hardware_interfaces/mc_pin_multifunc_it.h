/*

@startuml

namespace mc_interfaces {

interface PinMultifuncIt {
	{abstract}{method}+	bool	checkIt	( void )
	{abstract}{method}+	void	clearIt	( void )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include <stdint.h>

namespace mc_interfaces {

/*!
 * Класс предназначен для работы с прерываниями,
 * возникающими на линии в режиме работы на вход.
 */
class PinMultifuncIt {
public:
    /*!
     * Возвращает флаг зафиксированного прерывания на выводе.
     * \return		{	true	-	прерывание было зафиксировано.
     *					false	-	в противном случае.	}
     */
    virtual bool checkIt (void) = 0;
    
    /// Сбрасывает флаг зафиксированного прерывания на выводе.
    virtual void clearIt (void) = 0;
};

}

#endif
