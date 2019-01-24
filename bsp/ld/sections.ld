SECTIONS
{
    /*-------------------------Основная программа------------------------*/

    /* Таблица векторов прерываний программы целевого устройства,
       выравниванная по 4 байта (во flash). */
    .section_user_code_irq_table :
    {
        . = ALIGN(4);                       /* Задаем счетчику адрес кратный 4-м байтам. */
        KEEP(*(.user_code_irq_table))       /* Отключаем оптимизацию сборщика мусора (KEEP). */
        /* Иначе говоря, даже если массив векторов прерываний
           не используется программой, его обязательно следует разместить во flash. */
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Располагаем необходимые для выполнения основной программы созданные компилятором секции. */
    .sections_required_by_the_compiler :
    {
        . = ALIGN(4);    *(.glue_7*)          /* Собственные методы-прослойки для ARMv7 */
        . = ALIGN(4);    *(.glue_7t*)
        . = ALIGN(4);    *(.vfp11_veneer*)    /* Прочие секции поддержки. */
        . = ALIGN(4);    *(.v4_bx*)
        . = ALIGN(4);    *(.iplt*)
        . = ALIGN(4);    *(.rel.dyn*)
        . = ALIGN(4);    *(.eh_frame*)        /* Для использования исключений в CPP. */
        . = ALIGN(4);    *(.eh_framehdr*)
        . = ALIGN(4);    *(.init*)            /* Методы, выполняющиеся в начале программа. */
        . = ALIGN(4);    *(.fini*)            /* Методы, исполняемые по окончании программы. */

        . = ALIGN(4);    *(.init_array*)      /* Данные для служебных библиотек и инициализаторов C++. */
        . = ALIGN(4);    *(.fini_array*)
        . = ALIGN(4);    *(.ARM.exidx*)       /* ARM magic sections. */


        . = ALIGN(4);    *main.cpp.obj(.comment*);
        . = ALIGN(4);    *main.cpp.obj(.ARM.attributes*);

        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.comment*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.comment*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.comment*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.comment*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.comment*);
        . = ALIGN(4);    *libSYSTEM_DUMMY.a:*(.comment*);
        . = ALIGN(4);    *libUSER_CODE.a:*(.comment*);

        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libSYSTEM_DUMMY.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libUSER_CODE.a:*(.ARM.attributes*);
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Код основной программы. */
    .section_user_code_text :
    {
        . = ALIGN(4);    *main.cpp.obj(.text*);

        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.text*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.text*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.text*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.text*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.text*);
        . = ALIGN(4);    *libSYSTEM_DUMMY.a:*(.text*);
        . = ALIGN(4);    *libUSER_CODE.a:*(.text*);
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Объекты с неизменяемыми значениями.
       Используемые из кода основного проекта. */
    .section_user_code_rodata :
    {
        . = ALIGN(4);    *main.cpp.obj(.rodata*);

        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.rodata*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.rodata*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.rodata*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.rodata*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.rodata*);
        . = ALIGN(4);    *libSYSTEM_DUMMY.a:*(.rodata*);
        . = ALIGN(4);    *libUSER_CODE.a:*(.rodata*);
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Начальные значения изменяемых сущностей.
       Используемых из кода основного проекта. */
    .section_user_code_data :
    {
        . = ALIGN(4);    *main.cpp.obj(.data*);

        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.data*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.data*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.data*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.data*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.data*);
        . = ALIGN(4);    *libSYSTEM_DUMMY.a:*(.data*);
        . = ALIGN(4);    *libUSER_CODE.a:*(.data*);
        . = ALIGN(4);
    } >RAM_MAIN_PROGRAMM AT> ROM_MAIN_PROGRAMM

    /* Изменяемые сущности, значение которых по умолчанию 0.
       Используемые из кода основного проекта */
    .section_user_code_bss :
    {
        . = ALIGN(4);    *main.cpp.obj(.bss*);

        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.bss*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.bss*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.bss*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.bss*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.bss*);
        . = ALIGN(4);    *libSYSTEM_DUMMY.a:*(.bss*);
        . = ALIGN(4);    *libUSER_CODE.a:*(.bss*);
        . = ALIGN(4);
    } >RAM_MAIN_PROGRAMM

    /*-------------------------Внешние библиотеки------------------------*/
    /* Код внешних библиотек. */
    .section_external_lib_text :
    {
        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.text*);
        . = ALIGN(4);    *libFATFS.a:*(.text*);
        . = ALIGN(4);    *libFREERTOS.a:*(.text*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.text*);
        . = ALIGN(4);
    } >ROM_EXTERNAL_LIBRARIES

    .section_external_lib_required_by_the_compiler :
    {
        /* Библиотеки, требуемые для поддержки
           базовых возможностей языка. */
        . = ALIGN(4);    *crti.o(.comment*);
        . = ALIGN(4);    *crti.o(.ARM.attributes*);

        /* Библиотеки, добавленные пользователем. */
        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.comment*);
        . = ALIGN(4);    *libFATFS.a:*(.comment*);
        . = ALIGN(4);    *libFREERTOS.a:*(.comment*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.comment*);

        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libFATFS.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libFREERTOS.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.ARM.attributes*);
        . = ALIGN(4);
    }  >ROM_EXTERNAL_LIBRARIES

    /* Объекты с неизменяемыми значениями.
       Используемые из кода внешних библиотек. */
    .section_external_lib_rodata :
    {
        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.rodata*);
        . = ALIGN(4);    *libFATFS.a:*(.rodata*);
        . = ALIGN(4);    *libFREERTOS.a:*(.rodata*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.rodata*);
        . = ALIGN(4);
    } >ROM_EXTERNAL_LIBRARIES

    /* Начальные значения изменяемых сущностей.
       Используемые из кода внешних библиотек. */
    .section_external_lib_data :
    {
        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.data*);
        . = ALIGN(4);    *libFATFS.a:*(.data*);
        . = ALIGN(4);    *libFREERTOS.a:*(.data*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.data*);
        . = ALIGN(4);
    } >RAM_EXTERNAL_LIBRARIES AT> ROM_EXTERNAL_LIBRARIES

    /* Изменяемые сущности, значение которых по умолчанию 0.
       Используемые из кода внешних библиотек. */
    .section_external_lib_bss :
    {
        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.bss*);
        . = ALIGN(4);    *libFATFS.a:*(.bss*);
        . = ALIGN(4);    *libFREERTOS.a:*(.bss*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.bss*);
        . = ALIGN(4);
    } >RAM_EXTERNAL_LIBRARIES

    /*-----------------------Библиотеки пользователя---------------------*/
    /* Код библиотек пользователя. */
    .section_user_lib_text :
    {
        . = ALIGN(4);    *libBUTTONS.a:*(.text*);
        . = ALIGN(4);    *libCHIPTUNE.a:*(.text*);
        . = ALIGN(4);    *libDIGITAL_POTENTIOMETER.a:*(.text*);
        . = ALIGN(4);    *libLCD_DRIVER.a:*(.text*);
        . = ALIGN(4);    *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.text*);
        . = ALIGN(4);    *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.text*);
        . = ALIGN(4);    *libMICROSD_LOW_LEVEL_DRIVER.a:*(.text*);
        . = ALIGN(4);    *libSHIFT_REGISTER.a:*(.text*);
        . = ALIGN(4);    *libWAVE_GENERATORS.a:*(.text*);
        . = ALIGN(4);
    } >ROM_USER_LIBRARIES

    .section_user_lib_required_by_the_compiler :
    {
        . = ALIGN(4);    *libBUTTONS.a:*(.comment*);
        . = ALIGN(4);    *libCHIPTUNE.a:*(.comment*);
        . = ALIGN(4);    *libDIGITAL_POTENTIOMETER.a:*(.comment*);
        . = ALIGN(4);    *libLCD_DRIVER.a:*(.comment*);
        . = ALIGN(4);    *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.comment*);
        . = ALIGN(4);    *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.comment*);
        . = ALIGN(4);    *libMICROSD_LOW_LEVEL_DRIVER.a:*(.comment*);
        . = ALIGN(4);    *libSHIFT_REGISTER.a:*(.comment*);
        . = ALIGN(4);    *libWAVE_GENERATORS.a:*(.comment*);

        . = ALIGN(4);    *libBUTTONS.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libCHIPTUNE.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libDIGITAL_POTENTIOMETER.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libLCD_DRIVER.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libMICROSD_LOW_LEVEL_DRIVER.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libSHIFT_REGISTER.a:*(.ARM.attributes*);
        . = ALIGN(4);    *libWAVE_GENERATORS.a:*(.ARM.attributes*);
        . = ALIGN(4);
    }  >ROM_EXTERNAL_LIBRARIES

    /* Объекты с неизменяемыми значениями.
       Используемые из кода библиотек пользователя. */
    .section_user_lib_rodata :
    {
        . = ALIGN(4);    *libBUTTONS.a:*(.rodata*);
        . = ALIGN(4);    *libCHIPTUNE.a:*(.rodata*);
        . = ALIGN(4);    *libDIGITAL_POTENTIOMETER.a:*(.rodata*);
        . = ALIGN(4);    *libLCD_DRIVER.a:*(.rodata*);
        . = ALIGN(4);    *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.rodata*);
        . = ALIGN(4);    *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.rodata*);
        . = ALIGN(4);    *libMICROSD_LOW_LEVEL_DRIVER.a:*(.rodata*);
        . = ALIGN(4);    *libSHIFT_REGISTER.a:*(.rodata*);
        . = ALIGN(4);    *libWAVE_GENERATORS.a:*(.rodata*);
        . = ALIGN(4);
    } >ROM_USER_LIBRARIES

    /* Начальные значения изменяемых сущностей.
       Используемые из кода библиотек пользователя. */
    .section_user_lib_data :
    {
        . = ALIGN(4);     *libBUTTONS.a:*(.data*);
        . = ALIGN(4);     *libCHIPTUNE.a:*(.data*);
        . = ALIGN(4);     *libDIGITAL_POTENTIOMETER.a:*(.data*);
        . = ALIGN(4);     *libLCD_DRIVER.a:*(.data*);
        . = ALIGN(4);     *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.data*);
        . = ALIGN(4);     *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.data*);
        . = ALIGN(4);     *libMICROSD_LOW_LEVEL_DRIVER.a:*(.data*);
        . = ALIGN(4);     *libSHIFT_REGISTER.a:*(.data*);
        . = ALIGN(4);     *libWAVE_GENERATORS.a:*(.data*);
        . = ALIGN(4);
    } >RAM_USER_LIBRARIES AT> ROM_USER_LIBRARIES

    /* Изменяемые сущности, значение которых по умолчанию 0.
       Используемые из кода библиотек пользователя. */
    .section_user_lib_bss :
    {
        . = ALIGN(4);     *libBUTTONS.a:*(.bss*);
        . = ALIGN(4);     *libCHIPTUNE.a:*(.bss*);
        . = ALIGN(4);     *libDIGITAL_POTENTIOMETER.a:*(.bss*);
        . = ALIGN(4);     *libLCD_DRIVER.a:*(.bss*);
        . = ALIGN(4);     *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.bss*);
        . = ALIGN(4);     *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.bss*);
        . = ALIGN(4);     *libMICROSD_LOW_LEVEL_DRIVER.a:*(.bss*);
        . = ALIGN(4);     *libSHIFT_REGISTER.a:*(.bss*);
        . = ALIGN(4);     *libWAVE_GENERATORS.a:*(.bss*);
        . = ALIGN(4);
    } >RAM_USER_LIBRARIES
}
