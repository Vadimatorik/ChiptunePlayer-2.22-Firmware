/* Под начальный стек до запуска планировщика у нас есть своя секция в RAM. */
__estack = ORIGIN(RAM_MAIN_PROGRAMM_STACK) + LENGTH(RAM_MAIN_PROGRAMM_STACK);

/* Размер начального стека. */
__stack_size = LENGTH(RAM_MAIN_PROGRAMM_STACK);

/* Программа начинается с метода Reset_Handler. */
ENTRY(Reset_Handler)

/* Описание секций. */
SECTIONS
{
    /*---------------------ROM область загрузчика------------------------*/
    .section_bootloader : ALIGN(4)
    {
        /* На данный момент загрузчик отсутствует.
           Поэтому на месте кода загрузчика будет находиться
           только таблица векторов прерываний основного проекта.
           Таблица векторов прерываний должна быть передана
           компоновщику как отдельный .o файл, а не как часть
           библиотеки.*/
        . = ALIGN(4);    KEEP(*(.user_code_isr_vector .user_code_isr_vector*))
        . = ALIGN(4);
    } >ROM_BOOTLOADER

    /*----------------ROM область под внешние библиотеки-----------------*/
    /* Код. */
    .section_external_libraries_text : ALIGN(4)
    {
        /* Библиотеки компилятора. */
        . = ALIGN(4);    *libstdc++_nano.a:*(.text .text*);
        . = ALIGN(4);    *libgcc.a:*(.text .text*);
        . = ALIGN(4);    *libg_nano.a:*(.text .text*);

        /* Библиотеки пользователя */
        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.text .text*);
        . = ALIGN(4);    *libFATFS.a:*(.text .text*);
        . = ALIGN(4);    *libFREERTOS.a:*(.text .text*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.text .text*);
        . = ALIGN(4);
    } >ROM_EXTERNAL_LIBRARIES

    /* Секции, созданные компилятором */
    .section_external_libraries_required_by_the_compiler : ALIGN(4)
    {
        /* Библиотеки компилятора. */
        . = ALIGN(4);    *libgcc.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libstdc++_nano.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libg_nano.a:*(.ARM.attributes .ARM.attributes*);

        /* Библиотеки пользователя */
        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libFATFS.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libFREERTOS.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);
    }  >ROM_EXTERNAL_LIBRARIES

    /* Сущности с неизменяемыми значениями. */
    .section_external_libraries_rodata : ALIGN(4)
    {
        /* Библиотеки компилятора. */
        . = ALIGN(4);    *libgcc.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libstdc++_nano.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libg_nano.a:*(.rodata .rodata*);

        /* Библиотеки пользователя */
        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libFATFS.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libFREERTOS.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.rodata .rodata*);
        . = ALIGN(4);
    } >ROM_EXTERNAL_LIBRARIES

    /*-----------------------Библиотеки пользователя---------------------*/
    /* Код. */
    .section_user_libraries_text : ALIGN(4)
    {
        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.text .text*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.text .text*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.text .text*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.text .text*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.text .text*);
        . = ALIGN(4);    *libUSER_STARTUP.a:*(.text .text*);
        . = ALIGN(4);    *libBUTTONS.a:*(.text .text*);
        . = ALIGN(4);    *libCHIPTUNE.a:*(.text .text*);
        . = ALIGN(4);    *libDIGITAL_POTENTIOMETER.a:*(.text .text*);
        . = ALIGN(4);    *libLCD_DRIVER.a:*(.text .text*);
        . = ALIGN(4);    *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.text .text*);
        . = ALIGN(4);    *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.text .text*);
        . = ALIGN(4);    *libMICROSD_LOW_LEVEL_DRIVER.a:*(.text .text*);
        . = ALIGN(4);    *libSHIFT_REGISTER.a:*(.text .text*);
        . = ALIGN(4);    *libWAVE_GENERATORS.a:*(.text .text*);
        . = ALIGN(4);    *libRUN_TIME_LOGGER.a:*(.text .text*);
        . = ALIGN(4);
    } >ROM_USER_LIBRARIES

    /* Секции, созданные компилятором */
    .section_user_libraries_required_by_the_compiler : ALIGN(4)
    {
        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libUSER_STARTUP.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libUSER_CODE.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libBUTTONS.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libCHIPTUNE.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libDIGITAL_POTENTIOMETER.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libLCD_DRIVER.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libMICROSD_LOW_LEVEL_DRIVER.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libSHIFT_REGISTER.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libWAVE_GENERATORS.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);    *libRUN_TIME_LOGGER.a:*(.ARM.attributes .ARM.attributes*);
        . = ALIGN(4);
    }  >ROM_EXTERNAL_LIBRARIES

    /* Сущности с неизменяемыми значениями. */
    .section_user_libraries_rodata : ALIGN(4)
    {
        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libUSER_STARTUP.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libBUTTONS.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libCHIPTUNE.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libDIGITAL_POTENTIOMETER.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libLCD_DRIVER.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libMICROSD_LOW_LEVEL_DRIVER.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libSHIFT_REGISTER.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libWAVE_GENERATORS.a:*(.rodata .rodata*);
        . = ALIGN(4);    *libRUN_TIME_LOGGER.a:*(.rodata .rodata*);
        . = ALIGN(4);
    } >ROM_USER_LIBRARIES

    /*-------------------------Основная программа------------------------*/
    /* Код. */
    .section_user_code_text : ALIGN(4)
    {
        . = ALIGN(4);    *(.text .text.*)
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Секции, созданные компилятором */
    .sections_user_code_required_by_the_compiler : ALIGN(4)
    {
        . = ALIGN(4);    *(.glue_7 .glue_7*)              /* Собственные методы-прослойки для ARMv7 */
        . = ALIGN(4);    *(.glue_7t .glue_7t*)
        . = ALIGN(4);    *(.vfp11_veneer .vfp11_veneer*)  /* Прочие секции поддержки. */
        . = ALIGN(4);    *(.v4_bx .v4_bx*)
        . = ALIGN(4);    *(.iplt .iplt*)
        . = ALIGN(4);    *(.rel.dyn .rel.dyn*)
        . = ALIGN(4);    KEEP(*(.eh_frame .eh_frame*))    /* Для использования исключений в CPP. */
        . = ALIGN(4);    *(.eh_framehdr .eh_framehdr*)
        . = ALIGN(4);    *(.ARM.attributes .ARM.attributes.*)       /* Песь оставшийся дополнительный код,
                                                                       созданный компилятором. */
        . = ALIGN(4);    *(vtable)					            /* C++ virtual tables */

        PROVIDE_HIDDEN (__preinit_array_start = .);             /* Список методов, вызываемых перед конструкторами. */
        . = ALIGN(4); KEEP(*(.preinit_array_sysinit .preinit_array_sysinit*))
        . = ALIGN(4); KEEP(*(.preinit_array_platform .preinit_array_platform.*))
        . = ALIGN(4); KEEP(*(.preinit_array .preinit_array.*))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        PROVIDE_HIDDEN (__init_array_start = .);                /* Вызов конструкторов глобальных объектов. */
        . = ALIGN(4);KEEP(*(SORT(.init_array.*)))
        . = ALIGN(4); KEEP(*(.init_array))
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_end = .);

        PROVIDE_HIDDEN (__fini_array_start = .);                /* Вызов деструкторов глобальных объектов. */
        . = ALIGN(4); KEEP(*(SORT(.fini_array.*)))
        . = ALIGN(4); KEEP(*(.fini_array))
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_end = .);

        . = ALIGN(4);    KEEP(*(.cfmconfig))
        . = ALIGN(4);    *(.after_vectors .after_vectors.*)
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Сущности с неизменяемыми значениями. */
    .section_user_code_rodata : ALIGN(4)
    {
        . = ALIGN(4);    *(.rodata .rodata.*)
        . = ALIGN(4);
    } >ROM_MAIN_PROGRAMM

    /* Для stack trace. */
    .ARM.exidx :
    {
        . = ALIGN(4);    *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        . = ALIGN(4);
    } > ROM_MAIN_PROGRAMM

    /*-------------------------------RAM-----------------------------*/
    /* Начальные значения изменяемых сущностей. */
    .section_external_libraries_data : ALIGN(4)
    {
        . = ALIGN(4);    __external_lib_data_start =  . ;

        /* Библиотеки компилятора. */
        . = ALIGN(4);    *libgcc.a:*(.data .data*);
        . = ALIGN(4);    *libstdc++_nano.a:*(.data .data*);
        . = ALIGN(4);    *libg_nano.a:*(.data .data*);

        /* Библиотеки пользователя */
        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.data .data*);
        . = ALIGN(4);    *libFATFS.a:*(.data .data*);
        . = ALIGN(4);    *libFREERTOS.a:*(.data .data*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.data .data*);

        . = ALIGN(4);    __external_lib_data_end =  . ;
    } >RAM_EXTERNAL_LIBRARIES AT> ROM_EXTERNAL_LIBRARIES

    /* Область с нулевыми начальными значениями в RAM */
    .section_external_libraries_bss : ALIGN(4)
    {
        . = ALIGN(4);    __external_lib_bss_start = .;

        /* Библиотеки компилятора. */
        . = ALIGN(4);    *libgcc.a:*(.bss .bss*);
        . = ALIGN(4);    *libstdc++_nano.a:*(.bss .bss*);
        . = ALIGN(4);    *libg_nano.a:*(*COMMON);

        . = ALIGN(4);    *libgcc.a:*(*COMMON);
        . = ALIGN(4);    *libstdc++_nano.a:*(*COMMON);
        . = ALIGN(4);    *libg_nano.a:*(*COMMON);

        /* Библиотеки пользователя */
        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(.bss .bss*);
        . = ALIGN(4);    *libFATFS.a:*(.bss .bss*);
        . = ALIGN(4);    *libFREERTOS.a:*(.bss .bss*);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(.bss .bss*);

        . = ALIGN(4);    *libSTM32F4_LOW_LEVEL_BY_ST.a:*(*COMMON);
        . = ALIGN(4);    *libFATFS.a:*(*COMMON);
        . = ALIGN(4);    *libFREERTOS.a:*(*COMMON);
        . = ALIGN(4);    *libMAKISE_GUI.a:*(*COMMON);

        . = ALIGN(4);    __external_lib_bss_end = .;
    } >RAM_EXTERNAL_LIBRARIES

    /* Начальные значения изменяемых сущностей. */
    .section_user_libraries_data : ALIGN(4)
    {
        . = ALIGN(4);    __user_lib_data_start =  . ;

        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.data .data*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.data .data*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.data .data*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.data .data*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.data .data*);
        . = ALIGN(4);    *libUSER_STARTUP.a:*(.data .data*);
        . = ALIGN(4);    *libBUTTONS.a:*(.data .data*);
        . = ALIGN(4);    *libCHIPTUNE.a:*(.data .data*);
        . = ALIGN(4);    *libDIGITAL_POTENTIOMETER.a:*(.data .data*);
        . = ALIGN(4);    *libLCD_DRIVER.a:*(.data .data*);
        . = ALIGN(4);    *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.data .data*);
        . = ALIGN(4);    *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.data .data*);
        . = ALIGN(4);    *libMICROSD_LOW_LEVEL_DRIVER.a:*(.data .data*);
        . = ALIGN(4);    *libSHIFT_REGISTER.a:*(.data .data*);
        . = ALIGN(4);    *libWAVE_GENERATORS.a:*(.data .data*);
        . = ALIGN(4);    *libRUN_TIME_LOGGER.a:*(.data .data*);

        . = ALIGN(4);    __user_lib_data_end =  . ;
    } >RAM_USER_LIBRARIES AT> ROM_USER_LIBRARIES

    .section_user_libraries_bss : ALIGN(4)
    {
        . = ALIGN(4);    __user_lib_bss_start = .;

        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.bss .bss*);
        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(.bss .bss*);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(.bss .bss*);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(.bss .bss*);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(.bss .bss*);
        . = ALIGN(4);    *libUSER_CODE.a:*(.bss .bss*);
        . = ALIGN(4);    *libBUTTONS.a:*(.bss .bss*);
        . = ALIGN(4);    *libCHIPTUNE.a:*(.bss .bss*);
        . = ALIGN(4);    *libDIGITAL_POTENTIOMETER.a:*(.bss .bss*);
        . = ALIGN(4);    *libLCD_DRIVER.a:*(.bss .bss*);
        . = ALIGN(4);    *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(.bss .bss*);
        . = ALIGN(4);    *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(.bss .bss*);
        . = ALIGN(4);    *libMICROSD_LOW_LEVEL_DRIVER.a:*(.bss .bss*);
        . = ALIGN(4);    *libSHIFT_REGISTER.a:*(.bss .bss*);
        . = ALIGN(4);    *libWAVE_GENERATORS.a:*(.bss .bss*);
        . = ALIGN(4);    *libUSER_FREERTOS_LEVEL.a:*(.bss .bss*);
        . = ALIGN(4);    *libRUN_TIME_LOGGER.a:*(.bss .bss*);

        . = ALIGN(4);    *libUSER_BSP_LEVEL.a:*(*COMMON);
        . = ALIGN(4);    *libMC_INTERRUPT.a:*(*COMMON);
        . = ALIGN(4);    *libMC_HARDWARE.a:*(*COMMON);
        . = ALIGN(4);    *libPCB_HARDWARE.a:*(*COMMON);
        . = ALIGN(4);    *libUSER_CODE.a:*(*COMMON);
        . = ALIGN(4);    *libBUTTONS.a:*(*COMMON);
        . = ALIGN(4);    *libCHIPTUNE.a:*(*COMMON);
        . = ALIGN(4);    *libDIGITAL_POTENTIOMETER.a:*(*COMMON);
        . = ALIGN(4);    *libLCD_DRIVER.a:*(*COMMON);
        . = ALIGN(4);    *libMAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK.a:*(*COMMON);
        . = ALIGN(4);    *libMC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32.a:*(*COMMON);
        . = ALIGN(4);    *libMICROSD_LOW_LEVEL_DRIVER.a:*(*COMMON);
        . = ALIGN(4);    *libSHIFT_REGISTER.a:*(*COMMON);
        . = ALIGN(4);    *libWAVE_GENERATORS.a:*(*COMMON);
        . = ALIGN(4);    *libRUN_TIME_LOGGER.a:*(.COMMON*);

        . = ALIGN(4);    __user_lib_bss_end = .;
    } >RAM_USER_LIBRARIES

    /* Начальные значения изменяемых сущностей. */
    .section_user_code_data : ALIGN(4)
    {
        . = ALIGN(4);    __user_code_data_start =  . ;

        . = ALIGN(4);    *(.data .data.*)

        . = ALIGN(4);    __user_code_data_end =  . ;
    } >RAM_MAIN_PROGRAMM AT> ROM_MAIN_PROGRAMM

    .section_user_code_bss : ALIGN(4)
    {
        . = ALIGN(4);    __bss_start__ = .;    __user_code_bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);    __bss_end__ = .;    __user_code_bss_end = .;
    } >RAM_MAIN_PROGRAMM

    __external_lib_data_in_rom_start = LOADADDR(.section_external_libraries_data);
    __user_lib_data_in_rom_start = LOADADDR(.section_user_libraries_data);
    __user_code_data_in_rom_start = LOADADDR(.section_user_code_data);

    /*-------------------------Отладочная информация-----------------*/
    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }

    /*
     * DWARF debug sections.
     * Symbols in the DWARF debugging sections are relative to the beginning
     * of the section so we begin them at 0.
     */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }

    .debug_macro  0 : { *(.debug_macro) }
    .debug_ranges  0 : { *(.debug_ranges) }
}
