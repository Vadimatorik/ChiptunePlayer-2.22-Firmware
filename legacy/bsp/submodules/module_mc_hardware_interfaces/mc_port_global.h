/*

@startuml

namespace mc_interfaces {

interface PortGlobal {
	{abstract}{method}+	res		reinitAllPorts	( void )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "mc_base.h"

namespace mc_interfaces {

/*!
 * Класс предназначен для работы со всеми
 * используемыми в проекте выводами одновременно.
 */
class PortGlobal {
public:
    /*!
     * Сбрасывает настройки всех выводов,
     * после чего инициализирует их заново.
     *
     * Замечание: Управление тактовыми сигналами
     * портов ввода/вывода осуществляется внутри
     * метода автоматически.
     *
     * \return		{	BASE_RESULT::OK			-	инициализация прошла успешно.
     *					BASE_RESULT::ERROR_INIT	-	в противном случае.	}
     */
    virtual res reinitAllPorts (void) = 0;
};

}

#endif
