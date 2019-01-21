/* Карта памяти микроконтроллера stm32f405rgt6 под проект ChiptunePlayer-2.22-MainBoard-v2-Firmware. */
MEMORY
{
    /* Сектора 0-1 отводятся загрузчику. */
    ROM_BOOTLOADER (RX)             : ORIGIN = 0x08000000, LENGTH = 32K

    /* Сектора 2 используется для хранения настроек пользователя. */
    ROM_SYSCFG_PAGE_1 (R)           : ORIGIN = 0x08008000, LENGTH = 16K

    /* Сектор 3 используется для хранения копии настроек пользователя. */
    ROM_SYSCFG_PAGE_2 (R)           : ORIGIN = 0x0800C000, LENGTH = 16K

    /* Сектор 4 зарезервирован. */
    ROM_RESERVE (R)                 : ORIGIN = 0x08010000, LENGTH = 16K

    /* Сектора 5, 6 предназначены для хранения основного кода приложения. */
    ROM_MAIN_PROGRAMM (RX)          : ORIGIN = 0x08020000, LENGTH = 256K

    /* Сектора 7, 8, 9 предназначены для хранения сторонник библиотек (FATFS, FREERTOS...). */
    ROM_EXTERNAL_LIBRARIES (RX)     : ORIGIN = 0x08060000, LENGTH = 384K

    /* Сектора 10, 11 предназначены для хранения собственных библиотек
       (драйвера дисплеев, графические объекты...). */
    ROM_USER_LIBRARIES (RX)         : ORIGIN = 0x080C0000, LENGTH = 256K

    /* В режиме выполнения загрузчика карта RAM перекрывает карту целевого устройства. */
    RAM_PAGE_1 (RW)                 : ORIGIN = 0x20000000, LENGTH = 112K
    RAM_PAGE_2 (RW)                 : ORIGIN = 0x2001C000, LENGTH = 16K
    RAM_PAGE_3 (RW)                 : ORIGIN = 0x10000000, LENGTH = 64K

    /* С этой областью RAM будет работать код пользователя. */
    RAM_MAIN_PROGRAMM (RW)          : ORIGIN = 0x10000000, LENGTH = 64K

    /* Немного памяти RAM выделяется под начальный стек.
       До запуска планировщика FreeRTOS. */
    RAM_MAIN_PROGRAMM_STACK (RW)    : ORIGIN = 0x1000FC00, LENGTH = 1K

    /* С этой областью памяти будут работать сторонние библиотеки на манер FATFS или FreeRTOS. */
    RAM_EXTERNAL_LIBRARIES (RW)     : ORIGIN = 0x20000000, LENGTH = 100K

    /* С этой областью памяти будут работать библиотеки пользователя. */
    RAM_USER_LIBRARIES (RW)         : ORIGIN = 0x20019000, LENGTH = 28K
}