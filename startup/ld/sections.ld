__estack = ORIGIN(RAM_STACK) + LENGTH(RAM_STACK);
__stack_size = LENGTH(RAM_STACK);
__heap_end = ORIGIN(RAM_STACK);
__heap_start = __estack;

/* Программа начинается с метода Reset_Handler. */
ENTRY(Reset_Handler)

/* Описание секций. */
SECTIONS
{
    .code : ALIGN(4)
    {
        . = ALIGN(4); KEEP(*(.isr_vector .isr_vector*))
        . = ALIGN(4); *(.text .text.*)
        . = ALIGN(4); *(.glue_7 .glue_7*)                  /* Собственные методы-прослойки для ARMv7 */
        . = ALIGN(4); *(.glue_7t .glue_7t*)
        . = ALIGN(4); *(.vfp11_veneer .vfp11_veneer*)      /* Прочие секции поддержки. */
        . = ALIGN(4); *(.v4_bx .v4_bx*)
        . = ALIGN(4); *(.iplt .iplt*)
        . = ALIGN(4); *(.rel.dyn .rel.dyn*)
        . = ALIGN(4); KEEP(*(.eh_frame .eh_frame*))        /* Для использования исключений в CPP. */
        . = ALIGN(4); *(.eh_framehdr .eh_framehdr*)
        . = ALIGN(4); *(.ARM.attributes .ARM.attributes.*) /* Dесь оставшийся дополнительный код,
                                                              созданный компилятором. */
        . = ALIGN(4); *(vtable)                            /* C++ virtual tables */

        PROVIDE_HIDDEN (__preinit_array_start = .);        /* Список методов, вызываемых перед конструкторами. */
        . = ALIGN(4); KEEP(*(.preinit_array_sysinit .preinit_array_sysinit*))
        . = ALIGN(4); KEEP(*(.preinit_array_platform .preinit_array_platform.*))
        . = ALIGN(4); KEEP(*(.preinit_array .preinit_array.*))
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_end = .);

        PROVIDE_HIDDEN (__init_array_start = .);           /* Вызов конструкторов глобальных объектов. */
        . = ALIGN(4); KEEP(*(SORT(.init_array.*)))
        . = ALIGN(4); KEEP(*(.init_array))
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_end = .);

        PROVIDE_HIDDEN (__fini_array_start = .);           /* Вызов деструкторов глобальных объектов. */
        . = ALIGN(4); KEEP(*(SORT(.fini_array.*)))
        . = ALIGN(4); KEEP(*(.fini_array))
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_end = .);

        . = ALIGN(4); KEEP(*(.cfmconfig))
        . = ALIGN(4); *(.after_vectors .after_vectors.*)
        . = ALIGN(4);
    } >FLASH

    .rodata : ALIGN(4)
    {
        . = ALIGN(4); *(.rodata .rodata.*)
    } >FLASH

    .data : ALIGN(4)
    {
        . = ALIGN(4); __data_start =  . ;
        . = ALIGN(4); *(.data .data.*)
        . = ALIGN(4); __data_end =  . ;
    } >RAM AT> FLASH

    .bss : ALIGN(4)
    {
        . = ALIGN(4); __bss_start__ = .;    __bss_start = .;
        *(.bss .bss.*)
        *(COMMON) . = ALIGN(4);
         __bss_end__ = .;    __bss_end = .;
    } >RAM

    __data_in_rom_start = LOADADDR(.data);

    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH

    __exidx_start = .;
    .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } >FLASH
    __exidx_end = .;

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
