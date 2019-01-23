CMAKE_MINIMUM_REQUIRED (VERSION 3.13.2)

SET (CMAKE_SYSTEM_NAME Generic)
SET (CMAKE_SYSTEM_VERSION 1)

# Настройки компилятора.
SET (TOOLCHAIN_BIN_PATH /opt/arm/gcc-arm-none-eabi-7-2018-q2-update/bin)
SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)
SET (CMAKE_C_COMPILER ${TOOLCHAIN_BIN_PATH}/arm-none-eabi-gcc)
SET (CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_PATH}/arm-none-eabi-g++)
SET (ARM_SIZE "${TOOLCHAIN_BIN_PATH}/arm-none-eabi-size")
SET (ARM_OBJDUMP "${TOOLCHAIN_BIN_PATH}/arm-none-eabi-objdump")

PROJECT(ChiptunePlayer-2.22 C CXX ASM)

# Флаги конфигурации аппаратной части (параметры архитектуры).
SET (MICROCONTROLLER_HARDWARE_FLAGS "-mcpu=cortex-m4;-mthumb;-mfloat-abi=hard;-mfpu=fpv4-sp-d16;--specs=nano.specs")

SET (MEM_LD "${CMAKE_CURRENT_SOURCE_DIR}/bsp/ld/mem.ld")
SET (SECTIONS_LD "${CMAKE_CURRENT_SOURCE_DIR}/bsp/ld/sections.ld")


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

SET (BUTTON_PATH "${CMAKE_SOURCE_DIR}/bsp/submodule/module_button/buttons_core/inc")

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

SET (MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_PATH
     "${CMAKE_SOURCE_DIR}/bsp/submodule/module_makise_gui_elements_by_vadimatorik/inc")

# Путь до структур chiptune классов.
SET (CHIPTUNE_H_PATH "${CMAKE_SOURCE_DIR}/bsp/submodule/module_chiptune/ay_ym_file_play/inc"
                      "${CMAKE_SOURCE_DIR}/bsp/submodule/module_chiptune/ay_ym_low_lavel/inc"
                      "${CMAKE_SOURCE_DIR}/bsp/submodule/module_chiptune/ay_ym_note_mode/inc")

# Путь до структуры с объектами FreeRTOS пользователя.
SET (USER_FREERTOS_OBJ_H_PATH "${CMAKE_SOURCE_DIR}/bsp/freertos/user_obj/inc")

# Путь до генератора миандра.
SET (WAVE_GENERATORS_PATH "${CMAKE_SOURCE_DIR}/bsp/submodule/module_wave_generators/ltc6903/inc")

# Путь до lcd.
SET (LCD_DRIVER_PATH "${CMAKE_SOURCE_DIR}/bsp/submodule/module_lcd_driver"
                     "${CMAKE_SOURCE_DIR}/bsp/submodule/module_lcd_driver/gdeh0154d27/inc"
                     "${CMAKE_SOURCE_DIR}/bsp/submodule/module_lcd_driver/ssd1306/inc"
                     "${CMAKE_SOURCE_DIR}/bsp/submodule/module_lcd_driver/st7565/inc")

# Путь до потенциометров.
SET (DIGITAL_POTENTIOMETER_PATH "${CMAKE_SOURCE_DIR}/bsp/submodule/module_digital_potentiometer/ad5204/inc")

# Путь до потенциометров.
SET (MC_HARDWARE_PATH "${CMAKE_SOURCE_DIR}/bsp/mc_hardware/inc")

# Путь до потенциометров.
SET (PCB_HARDWARE_PATH "${CMAKE_SOURCE_DIR}/bsp/pcb_hardware/inc")

# Путь до потенциометров.
SET (FATFS_PATH "${CMAKE_SOURCE_DIR}/bsp/submodule/module_fatfs_by_chan/inc")

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

SET (RUN_TIME_LOGGER_OPTIMIZATION_FLAGS "-Ofast;-g0;")

SET (MC_HARDWARE_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (USER_FREERTOS_LEVEL_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (PCB_HARDWARE_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (MC_INTERRUPT_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (USER_BSP_LEVEL_OPTIMIZATION_FLAGS "-Ofast;-g0;")

SET (${PROJECT_NAME}_OPTIMIZATION_FLAGS "-Ofast;-g0;")

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

SET (RUN_TIME_LOGGER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

SET (MC_HARDWARE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (PCB_HARDWARE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (USER_FREERTOS_LEVEL_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (MC_INTERRUPT_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (USER_BSP_LEVEL_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

SET (${PROJECT_NAME}_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

# Пути к конфигурационным файлам библиотек.
SET (FATFS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (FREERTOS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (STM32F4_LOW_LEVEL_BY_ST_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (MAKISE_GUI_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

SET (RUN_TIME_LOGGER_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

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

add_subdirectory(${CMAKE_SOURCE_DIR}/user/submodule/module_run_time_logger)

add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/mc_hardware)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/freertos)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/pcb_hardware)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/interrupt)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/gui)

#add_subdirectory(${CMAKE_SOURCE_DIR}/user)



# Для сборки требуется:
include_directories(
        ${USER_OS_H_PATH} # Путь до файла user_os с прослойками под конкретную операционную систему.
        ${USER_OS_PATH} # Путь к заголовочным файлам операционной системы.
        ${MODULE_MC_HARDWARE_INTERFACES_PATH}
        ${MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_PATH}
        ${LOW_LEVEL_H_FILES_PATH}
        ${SD_H_PATH}
        ${USER_FREERTOS_OBJ_H_PATH}
        ${CHIPTUNE_H_PATH}
        ${SHIFT_REGISTER_BASE_PATH}
        ${WAVE_GENERATORS_PATH}
        ${LCD_DRIVER_PATH}
        ${DIGITAL_POTENTIOMETER_PATH}
        ${MC_HARDWARE_PATH}
        ${BUTTON_PATH}
        ${FATFS_PATH}
        ${PCB_HARDWARE_PATH}
        ${MAKISE_E_PATH}
        ${MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_PATH}
        ${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/ayplayer_file/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/base/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/fat/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/gui/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/gui/elements/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/muxer/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/nvic/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/rcc/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/sd_control/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/structs
        ${CMAKE_CURRENT_SOURCE_DIR}/user/submodule/module_run_time_logger/inc
)


# Компиляции полежат все <<.cpp>> файлы из директории src.
file (GLOB ${PROJECT_NAME}_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/user/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/ayplayer_file/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/base/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/fat/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/gui/elements/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/gui/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/muxer/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/nvic/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_class/sd_control/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/user/ayplayer_obj/*.cpp")

add_executable(${PROJECT_NAME}.elf ${${PROJECT_NAME}_SOURCES})


target_compile_options (${PROJECT_NAME}.elf PRIVATE
                        "${${PROJECT_NAME}_COMPILER_FLAGS}${${PROJECT_NAME}_OPTIMIZATION_FLAGS}")


add_custom_command (TARGET ${PROJECT_NAME}.elf PRE_BUILD
        COMMENT "${CMAKE_EXE_LINKER_FLAGS}")

target_link_libraries (${PROJECT_NAME}.elf PUBLIC
                       USER_FREERTOS_LEVEL
                       USER_BSP_LEVEL
                       MC_INTERRUPT
                       MC_HARDWARE
                       PCB_HARDWARE
                       BUTTONS
                       CHIPTUNE
                       DIGITAL_POTENTIOMETER
                       FATFS
                       FREERTOS
                       LCD_DRIVER
                       MAKISE_GUI
                       MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK
                       MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32
                       MICROSD_LOW_LEVEL_DRIVER
                       SHIFT_REGISTER
                       STM32F4_LOW_LEVEL_BY_ST
                       SYSTEM_DUMMY
                       WAVE_GENERATORS)


SET (LD_FILES "-T${MEM_LD};-T${SECTIONS_LD}")

SET (LD_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};-Wl,--undefined=uxTopUsedPriority;-ffunction-sections;-Wl,--gc-sections")

target_link_options(${PROJECT_NAME}.elf PRIVATE "${LD_FLAGS};${LD_FILES}")

# Вывод размера итогового elf (цельного).
add_custom_command (TARGET ${PROJECT_NAME}.elf POST_BUILD
                    DEPENDS ${PROJECT_NAME}.elf
                    COMMENT "${PROJECT_NAME} size information:"
                    COMMAND cd ${PROJECT_BINARY_DIR}
                    COMMAND ${ARM_SIZE} ${PROJECT_NAME}.elf -t)