extern "C" {

#include <stdint.h>
#include <sys/types.h>

/*!
 * Следующие поля импортируются из linker скрипта.
 */

/*! Список методов, вызываемых перед конструкторами. */
extern void (*__preinit_array_start[]) (void) __attribute__((weak));
extern void (*__preinit_array_end[]) (void) __attribute__((weak));

/*! Вызов конструкторов глобальных объектов. */
extern void (*__init_array_start[]) (void) __attribute__((weak));
extern void (*__init_array_end[]) (void) __attribute__((weak));

/*! Вызов деструкторов глобальных объектов. */
extern void (*__fini_array_start[]) (void) __attribute__((weak));
extern void (*__fini_array_end[]) (void) __attribute__((weak));

/*!
 * BSS.
 */
extern uint32_t __external_lib_bss_start; /*! Сторонние библиотеки. */
extern uint32_t __external_lib_bss_end;
extern uint32_t __user_lib_bss_start; /*! Библиотеки пользователя. */
extern uint32_t __user_lib_bss_end;
extern uint32_t __user_code_bss_start; /*! Основной код. */
extern uint32_t __user_code_bss_end;

/*!
 * DATA
 */
extern uint32_t __external_lib_data_in_rom_start; /*! Сторонние библиотеки. */
extern uint32_t __external_lib_data_start;
extern uint32_t __external_lib_data_end;
extern uint32_t __user_lib_data_in_rom_start; /*! Библиотеки пользователя. */
extern uint32_t __user_lib_data_start;
extern uint32_t __user_lib_data_end;
extern uint32_t __user_code_data_in_rom_start; /*! Основной код. */
extern uint32_t __user_code_data_start;
extern uint32_t __user_code_data_end;

/// Метод инициализирует data область (заданную).
inline void __attribute__((always_inline))
__initialize_data (uint32_t *from, uint32_t *section_begin, uint32_t *section_end) {
    uint32_t *p = section_begin;
    while ((uint32_t)p < (uint32_t)section_end)
        *p++ = *from++;
}

/// Метод заполняет 0-ми bss область (заданную).
inline void __attribute__((always_inline))
__initialize_bss (uint32_t *section_begin, uint32_t *section_end) {
    uint32_t *p = section_begin;
    while (p < section_end)
        *p++ = 0;
}

/// Метод вызывает все необходимые методы
/// начальной инициализации объектов перед main.
inline void __attribute__((always_inline)) __run_init_array (void) {
    int count, i;
    
    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
        __preinit_array_start[i]();
    
    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
        __init_array_start[i]();
}

/// Метод производит вызов деструкторов глобальных объектов.
inline void __attribute__((always_inline)) __run_destructors_array (void) {
    int count, i;
    count = __fini_array_end - __fini_array_start;
    for (i = 0; i < count; i++)
        __fini_array_start[i]();
}

/// Метод должен перезагрузить микроконтроллер.
extern void __attribute__((noreturn)) _exit (int);

/// Начало основной программы целевого устройства.
extern int main (void);

/// Метод производит инициализацию аппаратной периферии.
extern void SystemInit (void);

/// Для успешной компиляции.
void *__dso_handle;

/// С данного метода начинается выполнение программы.
/// Здесь происходит предварительная инициализация,
/// запуск main и последующая "уборка"
/// (вызов деструкторов глобальных объектов).
void __start (void) {
    /// BSS.
    __initialize_bss(&__external_lib_bss_start, &__external_lib_bss_end);
    __initialize_bss(&__user_lib_bss_start, &__user_lib_bss_end);
    __initialize_bss(&__user_code_bss_start, &__user_code_bss_end);
    
    /// DATA.
    __initialize_data(&__external_lib_data_in_rom_start, &__external_lib_data_start, &__external_lib_data_end);
    __initialize_data(&__user_lib_data_in_rom_start, &__user_lib_data_start, &__user_lib_data_end);
    __initialize_data(&__user_code_data_in_rom_start, &__user_code_data_start, &__user_code_data_end);
    
    /// Начальная инициализация глобальных объектов.
    __run_init_array();
    
    /// Сбрасываем периферию в приемлемое состояние по умолчанию.
    SystemInit();
    
    /// Начинаем выполнение основной программы.
    int code = main();
    
    /// Запускаем длеструкторы.
    __run_destructors_array();
    
    /// Выполняем перезагрузку.
    _exit(code);
    
    /// До сюда уже не дойдем.
    for (;;);
}

}