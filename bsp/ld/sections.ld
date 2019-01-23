SECTIONS
{
    /*-------------------------Основная программа------------------------*/

    /* Таблица векторов прерываний программы целевого устройства,
       выравниванная по 4 байта (во flash). */
    .section_user_code_irq_table :
    {
        . = ALIGN(4);               /* Задаем счетчику адрес кратный 4-м байтам. */
        KEEP(*(.irq_table))         /* Отключаем оптимизацию сборщика мусора (KEEP). */
        /* Иначе говоря, даже если массив векторов прерываний
           не используется программой, его обязательно следует разместить во flash. */
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Располагаем необходимые для выполнения основной программы созданные компилятором секции. */
    .sections_required_by_the_compiler :
    {
        . = ALIGN(4);
        *(.glue_7*)          /* Собственные методы-прослойки для ARMv7 */
        . = ALIGN(4);
        *(.glue_7t*)
        . = ALIGN(4);
        *(.vfp11_veneer*)    /* Прочие секции поддержки. */
        . = ALIGN(4);
        *(.v4_bx*)
        . = ALIGN(4);
        *(.iplt*)
        . = ALIGN(4);
        *(.rel.dyn*)
        . = ALIGN(4);
        *(.eh_frame*)        /* Для использования исключений в CPP. */
        . = ALIGN(4);
        *(.eh_framehdr*)
        . = ALIGN(4);
        *(.init*)            /* Методы, выполняющиеся в начале программа. */
        . = ALIGN(4);
        *(.fini*)            /* Методы, исполняемые по окончании программы. */
        . = ALIGN(4);
        *(.comment*)         /* Информация о версии компилятора. */
        . = ALIGN(4);
        *(.init_array*)      /* Данные для служебных библиотек и инициализаторов C++. */
        . = ALIGN(4);
        *(.fini_array*)
        . = ALIGN(4);
        *(.ARM.exidx*)       /* ARM magic sections. */
        . = ALIGN(4);
        *(.ARM.attributes*) /* ARM-специфичные атрибуты файла. */
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Код основной программы. */
    .section_user_code_text :
    {
        . = ALIGN(4);
        main.cpp.obj(.text)
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Объекты с неизменяемыми значениями.
       Используемые из кода основного проекта. */
    .section_user_code_rodata :
    {
        . = ALIGN(4);
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Начальные значения изменяемых сущностей.
       Используемых из кода основного проекта. */
    .section_user_code_data :
    {
        . = ALIGN(4);
        . = ALIGN(4);
    } >RAM_MAIN_PROGRAMM AT> ROM_MAIN_PROGRAMM

    /* Изменяемые сущности, значение которых по умолчанию 0.
       Используемые из кода основного проекта */
    .section_user_code_bss :
    {
        . = ALIGN(4);
        . = ALIGN(4);
    } >RAM_MAIN_PROGRAMM

    /*-------------------------Внешние библиотеки------------------------*/
    /* Код внешних библиотек. */
    .section_external_text :
    {
        . = ALIGN(4);

        . = ALIGN(4);
    } >ROM_EXTERNAL_LIBRARIES

    /* Объекты с неизменяемыми значениями.
       Используемые из кода внешних библиотек. */
    .section_external_rodata :
    {
        . = ALIGN(4);

        . = ALIGN(4);
    } >ROM_EXTERNAL_LIBRARIES

    /* Начальные значения изменяемых сущностей.
       Используемые из кода внешних библиотек. */
    .section_external_data :
    {
        . = ALIGN(4);

        . = ALIGN(4);
    } >RAM_EXTERNAL_LIBRARIES AT> ROM_EXTERNAL_LIBRARIES

    /* Изменяемые сущности, значение которых по умолчанию 0.
       Используемые из кода внешних библиотек. */
    .section_external_bss :
    {
        . = ALIGN(4);

        . = ALIGN(4);
    } >RAM_EXTERNAL_LIBRARIES

    /*-----------------------Библиотеки пользователя---------------------*/
    /* Код библиотек пользователя. */
    .section_external_text :
    {
        . = ALIGN(4);

        . = ALIGN(4);
    } >ROM_USER_LIBRARIES

    /* Объекты с неизменяемыми значениями.
       Используемые из кода библиотек пользователя. */
    .section_external_rodata :
    {
        . = ALIGN(4);

        . = ALIGN(4);
    } >ROM_USER_LIBRARIES

    /* Начальные значения изменяемых сущностей.
       Используемые из кода библиотек пользователя. */
    .section_external_data :
    {
        . = ALIGN(4);

        . = ALIGN(4);
    } >RAM_USER_LIBRARIES AT> ROM_USER_LIBRARIES

    /* Изменяемые сущности, значение которых по умолчанию 0.
       Используемые из кода библиотек пользователя. */
    .section_external_bss :
    {
        . = ALIGN(4);

        . = ALIGN(4);
    } >RAM_USER_LIBRARIES
}
