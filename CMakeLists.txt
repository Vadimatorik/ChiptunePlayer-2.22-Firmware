CMAKE_MINIMUM_REQUIRED(VERSION 3.13.2)

include(cmake_functions.cmake)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# Настройки компилятора.
SET(TOOLCHAIN_BIN_PATH /opt/arm/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-)
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_PATH}gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_PATH}g++)
SET(ARM_SIZE "${TOOLCHAIN_BIN_PATH}size")
SET(ARM_OBJDUMP "${TOOLCHAIN_BIN_PATH}objdump")
SET(ARM_OBJCOPY "${TOOLCHAIN_BIN_PATH}objcopy")

SET(BIN_EXTRACTOR ${CMAKE_SOURCE_DIR}/bin_extractor/build/bin_extractor)

# STM32PROG.
SET(STM32PROG_PORT "SWD")
SET(STM32PROG_FREQ "4000")

# Секции.
SET(SECTION_BOOTLOADER_ADDRESS "0x08000000")
SET(SECTION_SYSCFG_PAGE_1_ADDRESS "0x08008000")
SET(SECTION_EXTERNAL_LIB_ADDRESS "0x08020000")
SET(SECTION_USER_LIBRARIES_ADDRESS "0x08080000")
SET(SECTION_USER_CODE_ADDRESS "0x080E0000")
SET(ADDR_END_FLASH "0x08100000")

PROJECT(ChiptunePlayer-2.22 C CXX ASM)

# Флаги конфигурации аппаратной части (параметры архитектуры).
SET(MICROCONTROLLER_HARDWARE_FLAGS "-mcpu=cortex-m4;"
        "-mthumb;"
        "-mfloat-abi=hard"
        "-mfpu=fpv4-sp-d16;"
        "--specs=nano.specs;"
        "-fdata-sections"
        "-ffunction-sections"
        "-Wl,--gc-sections")

SET(MEM_LD "${CMAKE_CURRENT_SOURCE_DIR}/bsp/ld/mem.ld")
SET(SECTIONS_LD "${CMAKE_CURRENT_SOURCE_DIR}/bsp/ld/sections.ld")


SET(C_COMPILER_FLAGS "-std=gnu99;-fshort-enums")
SET(CPP_COMPILER_FLAGS "-Werror;-Wall;-Wextra;-std=c++14;-Wno-type-limits;-fno-exceptions;")


# Путь до файла user_os.h с прослойками под конкретную операционную систему.
SET(USER_OS_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)

# Путь к заголовочным файлам операционной системы.
SET(USER_OS_H_FILES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodules/module_freertos_for_stm32f4/inc)

# Путь до submodules с описанием интерфейсов.
SET(MODULE_MC_HARDWARE_INTERFACES_H_FILES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodules/module_mc_hardware_interfaces)

# Путь к реализациям интерфейсов под текущий микроконтроллер.
SET(MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_H_FILES_PATH
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_mc_hardware_interfaces_implementation_for_stm32/inc)

# Путь до заголовочных файлов работы с аппаратной периферией низкого уровня.
SET(LOW_LEVEL_H_FILES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodules/module_stm32f4_low_level_by_st/hal/inc
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_stm32f4_low_level_by_st/hal/inc/legacy
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_stm32f4_low_level_by_st/cmsis/device/inc
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_stm32f4_low_level_by_st/cmsis/inc)

SET(BUTTON_H_FILES_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_button/buttons_core/inc")

# Путь до сдвиговых регистров.
SET(MODULE_SHIFT_REGISTER_BASE_H_FILES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodules/module_shift_register/shift_register_base/inc
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_shift_register/shift_register_8bit_port/inc
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_shift_register/shift_register_pin/inc)

# Путь до SDIO.
SET(MODULE_SD_H_FILES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodules/module_microsd_low_level_driver
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_microsd_low_level_driver/microsd_card_sdio/inc
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_microsd_low_level_driver/microsd_card_spi/inc)

# Путь до необходимых заголовочных файлов MakiseGUI, чтобы можно было использовать базу элементов.
SET(MODULE_MAKISE_E_H_FILES_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI/fonts"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI/gui"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI/gui/styles"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI/gui/elements"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI/gui/elements/inc")

SET(MODULE_MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_H_FILES_PATH
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui_elements_by_vadimatorik/inc")

SET(MODULE_RUN_TIME_LOGGER_H_FILES_PATH "")

# Путь до структур chiptune классов.
SET(MODULE_CHIPTUNE_H_FILES_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/aym/file_play/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/aym/file_play/readers/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/aym/low_lavel/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/aym/note_mode/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/aym/file_play/mc_file_container/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/submodules/zxtune/include"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/submodules/zxtune/src/"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/submodules/zxtune/src/binary"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/submodules/zxtune/src/formats/chiptune/aym")

# Путь до структуры с объектами FreeRTOS пользователя.
SET(USER_FREERTOS_OBJ_H_FILES_PATH "${CMAKE_SOURCE_DIR}/bsp/freertos/user_obj/inc")

# Путь до генератора миандра.
SET(MODULE_WAVE_GENERATORS_H_FILES_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_wave_generators/ltc6903/inc")

# Путь до lcd.
SET(MODULE_LCD_DRIVER_H_FILES_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_lcd_driver"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_lcd_driver/gdeh0154d27/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_lcd_driver/ssd1306/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_lcd_driver/st7565/inc")

# Путь до потенциометров.
SET(MODULE_DIGITAL_POTENTIOMETER_H_FILES_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_digital_potentiometer/ad5204/inc")

# Путь до потенциометров.
SET(MC_HARDWARE_H_FILES_PATH "${CMAKE_SOURCE_DIR}/bsp/mc_hardware/inc")

# Путь до потенциометров.
SET(PCB_HARDWARE_H_FILES_PATH "${CMAKE_SOURCE_DIR}/bsp/pcb_hardware/inc")

# Путь до потенциометров.
SET(MODULE_FATFS_H_FILES_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_fatfs_by_chan/inc")

# Флаги оптимизации библиотек.
SET(MODULE_FATFS_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_FREERTOS_OPTIMIZATION_FLAGS "-O0;-g3;")

SET(MODULE_STM32F4_LOW_LEVEL_BY_ST_OPTIMIZATION_FLAGS "-O0;-g3;")

SET(MODULE_BUTTONS_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_DIGITAL_POTENTIOMETER_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_CHIPTUNE_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_SHIFT_REGISTER_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_LCD_DRIVER_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_MAKISE_GUI_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_MODULE__OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_MICROSD_LOW_LEVEL_DRIVER_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_WAVE_GENERATORS_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_RUN_TIME_LOGGER_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MODULE_MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_OPTIMIZATION_FLAGS "-O0;-g3;")


SET(MC_HARDWARE_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(USER_FREERTOS_LEVEL_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(PCB_HARDWARE_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MC_INTERRUPT_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(USER_BSP_LEVEL_OPTIMIZATION_FLAGS "-O0;-g3;")

SET(USER_CODE_OPTIMIZATION_FLAGS "-O0;-g3;")

# Оставшиеся параметры сборки библиотек.
SET(MODULE_FATFS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET(MODULE_FREERTOS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET(MODULE_STM32F4_LOW_LEVEL_BY_ST_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET(MODULE_MAKISE_GUI_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET(MODULE_MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_COMPILER_FLAGS
        "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")

SET(MODULE_BUTTONS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32_COMPILER_FLAGS
        "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MODULE_DIGITAL_POTENTIOMETER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MODULE_CHIPTUNE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MODULE_SHIFT_REGISTER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MODULE_LCD_DRIVER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MODULE_MICROSD_LOW_LEVEL_DRIVER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MODULE_WAVE_GENERATORS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

SET(MODULE_RUN_TIME_LOGGER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

SET(MC_HARDWARE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(PCB_HARDWARE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(USER_FREERTOS_LEVEL_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MC_INTERRUPT_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(USER_BSP_LEVEL_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

SET(USER_CODE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

# Пути к конфигурационным файлам библиотек.
SET(MODULE_FATFS_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_FREERTOS_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_STM32F4_LOW_LEVEL_BY_ST_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_MAKISE_GUI_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)

SET(MODULE_RUN_TIME_LOGGER_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)

SET(MODULE_BUTTONS_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_DIGITAL_POTENTIOMETER_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_CHIPTUNE_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_SHIFT_REGISTER_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_LCD_DRIVER_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_MICROSD_LOW_LEVEL_DRIVER_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MODULE_WAVE_GENERATORS_CFG_H_FILES_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(LIB_CFG_H_FILES_PATH {CMAKE_SOURCE_DIR}/cfg)

# Сборка требуемых библиотек.
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_fatfs_by_chan)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_freertos_for_stm32f4)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_stm32f4_low_level_by_st)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui_elements_by_vadimatorik)

add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_button)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_mc_hardware_interfaces_implementation_for_stm32)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_shift_register)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_digital_potentiometer)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_lcd_driver)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_microsd_low_level_driver)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodules/module_wave_generators)

add_subdirectory(${CMAKE_SOURCE_DIR}/user_code/submodules/module_run_time_logger)

add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/mc_hardware)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/freertos)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/pcb_hardware)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/interrupt)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/gui)

add_subdirectory(${CMAKE_SOURCE_DIR}/user_code)

add_executable(${PROJECT_NAME}.elf "user_code/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/bsp/startup/src/vectors.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/bsp/startup/src/loader.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/bsp/submodules/module_system_dummy/src/cpp_system_calls.cpp")

include_directories(
        ${USER_OS_H_FILES_PATH} # Путь до файла user_os с прослойками под конкретную операционную систему.
        ${USER_OS_H_FILES_PATH} # Путь к заголовочным файлам операционной системы.
        ${PCB_HARDWARE_H_FILES_PATH}

        ${MODULE_MC_HARDWARE_INTERFACES_H_FILES_PATH}
        ${MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_H_FILES_PATH}
        ${MODULE_LOW_LEVEL_H_FILES_PATH}
        ${MODULE_SD_H_FILES_PATH}
        ${MODULE_USER_FREERTOS_OBJ_H_FILES_PATH}
        ${MODULE_CHIPTUNE_H_FILES_PATH}
        ${MODULE_SHIFT_REGISTER_BASE_H_FILES_PATH}
        ${MODULE_WAVE_GENERATORS_H_FILES_PATH}
        ${MODULE_LCD_DRIVER_H_FILES_PATH}
        ${MODULE_DIGITAL_POTENTIOMETER_H_FILES_PATH}
        ${MODULE_MC_HARDWARE_H_FILES_PATH}
        ${MODULE_BUTTON_H_FILES_PATH}
        ${MODULE_FATFS_H_FILES_PATH}
        ${MODULE_MAKISE_E_H_FILES_PATH}
        ${MODULE_MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_H_FILES_PATH}

        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_system_dummy/inc
        ${CMAKE_SOURCE_DIR}/bsp/startup/inc
        ${CMAKE_SOURCE_DIR}/user_code/ayplayer_class/ayplayer_file/inc
        ${CMAKE_SOURCE_DIR}/user_code/ayplayer_class/base/inc
        ${CMAKE_SOURCE_DIR}/user_code/ayplayer_class/fat/inc
        ${CMAKE_SOURCE_DIR}/user_code/ayplayer_class/gui/inc
        ${CMAKE_SOURCE_DIR}/user_code/ayplayer_class/gui/elements/inc
        ${CMAKE_SOURCE_DIR}/user_code/ayplayer_class/muxer/inc
        ${CMAKE_SOURCE_DIR}/user_code/ayplayer_class/nvic/inc
        ${CMAKE_SOURCE_DIR}/user_code/ayplayer_class/rcc/inc
        ${CMAKE_SOURCE_DIR}/user_code/ayplayer_class/sd_control/inc
        ${CMAKE_SOURCE_DIR}/user_code/ayplayer_class/structs
        ${CMAKE_SOURCE_DIR}/user_code/submodules/module_run_time_logger/inc
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
        "${${PROJECT_NAME}_COMPILER_FLAGS}${${PROJECT_NAME}_OPTIMIZATION_FLAGS}")

add_custom_command(TARGET ${PROJECT_NAME}.elf PRE_BUILD
        COMMENT "${CMAKE_EXE_LINKER_FLAGS}")

target_link_libraries(${PROJECT_NAME}.elf PUBLIC
        "-Wl,--start-group"
        USER_CODE
        USER_FREERTOS_LEVEL
        USER_BSP_LEVEL
        MC_INTERRUPT
        MC_HARDWARE
        PCB_HARDWARE

        MODULE_BUTTONS
        MODULE_CHIPTUNE
        MODULE_DIGITAL_POTENTIOMETER
        MODULE_LCD_DRIVER
        MODULE_MAKISE_GUI_ELEMENTS_BY_VADIMATORIK
        MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32
        MODULE_MICROSD_LOW_LEVEL_DRIVER
        MODULE_SHIFT_REGISTER
        MODULE_WAVE_GENERATORS
        MODULE_STM32F4_LOW_LEVEL_BY_ST
        MODULE_FATFS
        MODULE_FREERTOS
        MODULE_MAKISE_GUI
        RUN_TIME_LOGGER
        "-Wl,--end-group")

SET(LD_FILES "-T${MEM_LD};-T${SECTIONS_LD};")
SET(LD_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};"
        "-Wl,--undefined=uxTopUsedPriority;"
        "-Wl,--gc-sections;"
        "-Wl,--strip-discarded;"
        "-Wl,--no-dynamic-linker;"
        "-nostartfiles")

# Компиляция файлов происходит по флагам из корневого CMakeList.
target_compile_options(${PROJECT_NAME}.elf PRIVATE
        "${USER_CODE_COMPILER_FLAGS}${USER_CODE_OPTIMIZATION_FLAGS}")

target_link_options(${PROJECT_NAME}.elf PRIVATE
        "${LD_FLAGS};"
        "${LD_FILES};"
        "-Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "${PROJECT_NAME} size information:"
        COMMAND ${ARM_SIZE} ${PROJECT_NAME}.elf -t)

if (ST_FLASH_USE STREQUAL "ON")
    write_sector("bootloader" ${SECTION_BOOTLOADER_ADDRESS} ${SECTION_BOOTLOADER_ADDRESS} ${SECTION_SYSCFG_PAGE_1_ADDRESS})
    write_sector("external_libraries" ${SECTION_BOOTLOADER_ADDRESS} ${SECTION_EXTERNAL_LIB_ADDRESS} ${SECTION_USER_LIBRARIES_ADDRESS})
    write_sector("user_libraries" ${SECTION_BOOTLOADER_ADDRESS} ${SECTION_USER_LIBRARIES_ADDRESS} ${SECTION_USER_CODE_ADDRESS})
    write_sector("main_programm" ${SECTION_BOOTLOADER_ADDRESS} ${SECTION_USER_CODE_ADDRESS} ${ADDR_END_FLASH})
endif ()