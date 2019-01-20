CMAKE_MINIMUM_REQUIRED(VERSION 3.13.2)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# Настройки компилятора.
SET(TOOLCHAIN_BIN_PATH /opt/arm/gcc-arm-none-eabi-7-2018-q2-update/bin)
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_PATH}/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_PATH}/arm-none-eabi-g++)
SET(ARM_SIZE "${TOOLCHAIN_BIN_PATH}/arm-none-eabi-size")
SET(ARM_OBJDUMP "${TOOLCHAIN_BIN_PATH}/arm-none-eabi-objdump")

# Флаги конфигурации аппаратной части (параметры архитектуры).
SET(MICROCONTROLLER_HARDWARE_FLAGS "-mcpu=cortex-m4;-mthumb;-mfloat-abi=hard;-mfpu=fpv4-sp-d16;--specs=nano.specs")

PROJECT(ChiptunePlayer-2.22 C CXX ASM)

SET (C_COMPILER_FLAGS "-std=gnu99;-fshort-enums")
SET (CPP_COMPILER_FLAGS "-Werror;-Wall;-Wextra;-std=c++14;-fno-exceptions;-fshort-enums")

# Путь до файла user_os.h с прослойками под конкретную операционную систему.
SET (USER_OS_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

# Путь к заголовочным файлам операционной системы.
SET (USER_OS_PATH ${CMAKE_SOURCE_DIR}/bsp/submodule/module_freertos_for_stm32f4/inc)

# Путь до submodule с описанием интерфейсов.
SET (MODULE_MC_HARDWARE_INTERFACES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodule/module_mc_hardware_interfaces)

# Путь к реализациям интерфейсов под текущий микроконтроллер.
SET (MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_PATH
     ${CMAKE_SOURCE_DIR}/bsp/submodule/module_mc_hardware_interfaces_implementation_for_stm32/inc)

# Путь до заголовочных файлов работы с аппаратной периферией низкого уровня.
SET (LOW_LEVEL_H_FILES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodule/module_stm32f4_low_level_by_st/hal/inc
                            ${CMAKE_SOURCE_DIR}/bsp/submodule/module_stm32f4_low_level_by_st/hal/inc/legacy
                            ${CMAKE_SOURCE_DIR}/bsp/submodule/module_stm32f4_low_level_by_st/cmsis/device/inc
                            ${CMAKE_SOURCE_DIR}/bsp/submodule/module_stm32f4_low_level_by_st/cmsis/inc)

# Путь до сдвиговых регистров.
SET (SHIFT_REGISTER_BASE_PATH ${CMAKE_SOURCE_DIR}/bsp/submodule/module_shift_register/shift_register_base/inc
                              ${CMAKE_SOURCE_DIR}/bsp/submodule/module_shift_register/shift_register_8bit_port/inc
                              ${CMAKE_SOURCE_DIR}/bsp/submodule/module_shift_register/shift_register_pin/inc)

# Путь до SDIO.
SET (SD_H_PATH ${CMAKE_SOURCE_DIR}/bsp/submodule/module_microsd_low_level_driver
               ${CMAKE_SOURCE_DIR}/bsp/submodule/module_microsd_low_level_driver/microsd_card_sdio/inc
               ${CMAKE_SOURCE_DIR}/bsp/submodule/module_microsd_low_level_driver/microsd_card_spi/inc)

# Путь до необходимых заголовочных файлов MakiseGUI, чтобы можно было использовать базу элементов.
SET (MAKISE_E_PATH "${CMAKE_SOURCE_DIR}/bsp/submodule/module_makise_gui/MakiseGUI"
                   "${CMAKE_SOURCE_DIR}/bsp/submodule/module_makise_gui/MakiseGUI/fonts"
                   "${CMAKE_SOURCE_DIR}/bsp/submodule/module_makise_gui/MakiseGUI/gui"
                   "${CMAKE_SOURCE_DIR}/bsp/submodule/module_makise_gui/MakiseGUI/gui/styles"
                   "${CMAKE_SOURCE_DIR}/bsp/submodule/module_makise_gui/MakiseGUI/gui/elements"
                   "${CMAKE_SOURCE_DIR}/bsp/submodule/module_makise_gui/MakiseGUI/gui/elements/inc")

# Флаги оптимизации библиотек.
SET (FATFS_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (FREERTOS_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (SYSTEM_DUMMY_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (STM32F4_LOW_LEVEL_BY_ST_OPTIMIZATION_FLAGS "-Ofast;-g0;")

SET (BUTTONS_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (DIGITAL_POTENTIOMETER_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (CHIPTUNE_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (SHIFT_REGISTER_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (LCD_DRIVER_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (MAKISE_GUI_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (MICROSD_LOW_LEVEL_DRIVER_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (WAVE_GENERATORS_OPTIMIZATION_FLAGS "-Ofast;-g0;")

SET (MC_HARDWARE_OPTIMIZATION_FLAGS "-Ofast;-g0;")

# Оставшиеся параметры сборки библиотек.
SET (FATFS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET (FREERTOS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET (STM32F4_LOW_LEVEL_BY_ST_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET (MAKISE_GUI_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET (MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK_COMPILER_FLAGS
     "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")

SET (SYSTEM_DUMMY_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (BUTTONS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32_COMPILER_FLAGS
     "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (DIGITAL_POTENTIOMETER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (CHIPTUNE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (SHIFT_REGISTER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (LCD_DRIVER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (MICROSD_LOW_LEVEL_DRIVER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (WAVE_GENERATORS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

SET (MC_HARDWARE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

# Пути к конфигурационным файлам библиотек.
SET (FATFS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (FREERTOS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (STM32F4_LOW_LEVEL_BY_ST_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (MAKISE_GUI_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

SET (SYSTEM_DUMMY_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (BUTTONS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (DIGITAL_POTENTIOMETER_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (CHIPTUNE_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (SHIFT_REGISTER_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (LCD_DRIVER_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (MICROSD_LOW_LEVEL_DRIVER_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (WAVE_GENERATORS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

# Сборка требуемых библиотек.
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_fatfs_by_chan)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_freertos_for_stm32f4)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_stm32f4_low_level_by_st)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_makise_gui)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_makise_gui_elements_by_vadimatorik)

add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_system_dummy)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_button)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_mc_hardware_interfaces_implementation_for_stm32)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_shift_register)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_digital_potentiometer)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_chiptune)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_lcd_driver)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_microsd_low_level_driver)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_wave_generators)

add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/mc_hardware)
