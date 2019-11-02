#pragma once

#ifdef __cplusplus

#include <stdint.h>

namespace mc_interfaces {

/*!
 * Стандартные ответы от методов
 * интерфейсов микроконтроллера.
 */
enum class res {
    err_ok = 0, /// Операция успешна произведена.
    err_timeout = 1, /// Был произведён выход по истечении времени ожидания.
    err_len = 2, /// Попытка передать/принять пакет 0-й длины.
    err_init = 3, /// Ошибка инициализации/модуль не был ранее инициализирован.
    err_not_data = 4, /// Нет запрашиваемых данных.
    err_input_value = 5 /// Входящее значение ошибочно.
};

/*!
 * Шаблон позволяет прекратить последовательность
 * действий и выйти, если операция с интерфейсом
 * была произведена не удачно.
 */
#define        checkResult(baseResultVariable)                                                \
            if ( baseResultVariable != mc_interfaces::res::err_ok ) return baseResultVariable;
    
}

#endif
