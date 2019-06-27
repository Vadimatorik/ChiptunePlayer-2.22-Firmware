CMAKE_MINIMUM_REQUIRED(VERSION 3.13.2)

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

# STM32PROG.
set(STM32PROG_USE off)
SET(STM32PROG "/home/vadimatorik/u_prog/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI")
SET(STM32PROG_PORT "SWD")
SET(STM32PROG_FREQ "4000")

# Секции.
SET(SECTION_BOOTLOADER_ADDRESS "0x08000000")
SET(SECTION_EXTERNAL_LIB_ADDRESS "0x08020000")
SET(SECTION_USER_LIBRARIES_ADDRESS "0x08080000")
SET(SECTION_USER_CODE_ADDRESS "0x080E0000")

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
SET(USER_OS_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

# Путь к заголовочным файлам операционной системы.
SET(USER_OS_PATH ${CMAKE_SOURCE_DIR}/bsp/submodules/module_freertos_for_stm32f4/inc)

# Путь до submodules с описанием интерфейсов.
SET(MODULE_MC_HARDWARE_INTERFACES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodules/module_mc_hardware_interfaces)

# Путь к реализациям интерфейсов под текущий микроконтроллер.
SET(MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_PATH
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_mc_hardware_interfaces_implementation_for_stm32/inc)

# Путь до заголовочных файлов работы с аппаратной периферией низкого уровня.
SET(LOW_LEVEL_H_FILES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodules/module_stm32f4_low_level_by_st/hal/inc
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_stm32f4_low_level_by_st/hal/inc/legacy
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_stm32f4_low_level_by_st/cmsis/device/inc
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_stm32f4_low_level_by_st/cmsis/inc)

SET(BUTTON_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_button/buttons_core/inc")

# Путь до сдвиговых регистров.
SET(SHIFT_REGISTER_BASE_PATH ${CMAKE_SOURCE_DIR}/bsp/submodules/module_shift_register/shift_register_base/inc
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_shift_register/shift_register_8bit_port/inc
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_shift_register/shift_register_pin/inc)

# Путь до SDIO.
SET(SD_H_PATH ${CMAKE_SOURCE_DIR}/bsp/submodules/module_microsd_low_level_driver
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_microsd_low_level_driver/microsd_card_sdio/inc
        ${CMAKE_SOURCE_DIR}/bsp/submodules/module_microsd_low_level_driver/microsd_card_spi/inc)

# Путь до необходимых заголовочных файлов MakiseGUI, чтобы можно было использовать базу элементов.
SET(MAKISE_E_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI/fonts"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI/gui"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI/gui/styles"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI/gui/elements"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui/MakiseGUI/gui/elements/inc")

SET(MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_PATH
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_makise_gui_elements_by_vadimatorik/inc")

# Путь до структур chiptune классов.
SET(CHIPTUNE_H_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/ay_ym_file_play/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/ay_ym_low_lavel/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_chiptune/ay_ym_note_mode/inc")

# Путь до структуры с объектами FreeRTOS пользователя.
SET(USER_FREERTOS_OBJ_H_PATH "${CMAKE_SOURCE_DIR}/bsp/freertos/user_obj/inc")

# Путь до генератора миандра.
SET(WAVE_GENERATORS_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_wave_generators/ltc6903/inc")

# Путь до lcd.
SET(LCD_DRIVER_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_lcd_driver"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_lcd_driver/gdeh0154d27/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_lcd_driver/ssd1306/inc"
        "${CMAKE_SOURCE_DIR}/bsp/submodules/module_lcd_driver/st7565/inc")

# Путь до потенциометров.
SET(DIGITAL_POTENTIOMETER_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_digital_potentiometer/ad5204/inc")

# Путь до потенциометров.
SET(MC_HARDWARE_PATH "${CMAKE_SOURCE_DIR}/bsp/mc_hardware/inc")

# Путь до потенциометров.
SET(PCB_HARDWARE_PATH "${CMAKE_SOURCE_DIR}/bsp/pcb_hardware/inc")

# Путь до потенциометров.
SET(FATFS_PATH "${CMAKE_SOURCE_DIR}/bsp/submodules/module_fatfs_by_chan/inc")

# Флаги оптимизации библиотек.
SET(FATFS_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(FREERTOS_OPTIMIZATION_FLAGS "-O0;-g3;")

SET(STM32F4_LOW_LEVEL_BY_ST_OPTIMIZATION_FLAGS "-O0;-g3;")

SET(BUTTONS_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(DIGITAL_POTENTIOMETER_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(CHIPTUNE_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(SHIFT_REGISTER_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(LCD_DRIVER_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MAKISE_GUI_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MICROSD_LOW_LEVEL_DRIVER_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(WAVE_GENERATORS_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(RUN_TIME_LOGGER_OPTIMIZATION_FLAGS "-O0;-g3;")

SET(MC_HARDWARE_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(USER_FREERTOS_LEVEL_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(PCB_HARDWARE_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(MC_INTERRUPT_OPTIMIZATION_FLAGS "-O0;-g3;")
SET(USER_BSP_LEVEL_OPTIMIZATION_FLAGS "-O0;-g3;")

SET(USER_CODE_OPTIMIZATION_FLAGS "-O0;-g3;")

# Оставшиеся параметры сборки библиотек.
SET(FATFS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET(FREERTOS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET(STM32F4_LOW_LEVEL_BY_ST_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET(MAKISE_GUI_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET(MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK_COMPILER_FLAGS
        "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")

SET(BUTTONS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32_COMPILER_FLAGS
        "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(DIGITAL_POTENTIOMETER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(CHIPTUNE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(SHIFT_REGISTER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(LCD_DRIVER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MICROSD_LOW_LEVEL_DRIVER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(WAVE_GENERATORS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

SET(RUN_TIME_LOGGER_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

SET(MC_HARDWARE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(PCB_HARDWARE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(USER_FREERTOS_LEVEL_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(MC_INTERRUPT_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET(USER_BSP_LEVEL_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

SET(USER_CODE_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

# Пути к конфигурационным файлам библиотек.
SET(FATFS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(FREERTOS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(STM32F4_LOW_LEVEL_BY_ST_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MAKISE_GUI_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

SET(RUN_TIME_LOGGER_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

SET(BUTTONS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(DIGITAL_POTENTIOMETER_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(CHIPTUNE_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(SHIFT_REGISTER_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(LCD_DRIVER_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(MICROSD_LOW_LEVEL_DRIVER_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET(WAVE_GENERATORS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

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
        USER_FREERTOS_LEVEL
        USER_BSP_LEVEL
        MC_INTERRUPT
        MC_HARDWARE
        PCB_HARDWARE
        USER_CODE
        BUTTONS
        CHIPTUNE
        DIGITAL_POTENTIOMETER
        LCD_DRIVER
        MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_ELEMENTS_BY_VADIMATORIK
        MC_HARDWARE_INTERFACES_IMPLEMENTATION_FOR_STM32
        MICROSD_LOW_LEVEL_DRIVER
        SHIFT_REGISTER
        WAVE_GENERATORS
        STM32F4_LOW_LEVEL_BY_ST
        FATFS
        FREERTOS
        MAKISE_GUI
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

if (STM32PROG_USE STREQUAL "ON")
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "Creating a binary file of all section."
        COMMAND ${ARM_OBJCOPY} --output-target=binary ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_all.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "Creating a binary file of the <<bootloader>> section."
        COMMAND ${ARM_OBJCOPY} --output-target=binary --only-section=.section_bootloader ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_section_bootloader_new.bin
        COMMAND cd ${CMAKE_SOURCE_DIR} && ./cmp.sh ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_section_bootloader.bin ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_section_bootloader_new.bin "${STM32PROG} -c port=${STM32PROG_PORT} freq=${STM32PROG_FREQ} -w ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_section_bootloader.bin ${SECTION_BOOTLOADER_ADDRESS}")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "Creating a binary file of the <<user_code>> section."
        COMMAND ${ARM_OBJCOPY} --output-target=binary --only-section=.section_user_code_text --only-section=.section_user_code_required_by_the_compiler --only-section=.section_user_code_rodata --only-section=.ARM.exidx ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_user_code_new.bin
        COMMAND cd ${CMAKE_SOURCE_DIR} && ./cmp.sh ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_user_code.bin ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_user_code_new.bin "${STM32PROG} -c port=${STM32PROG_PORT} freq=${STM32PROG_FREQ} -w ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_user_code.bin ${SECTION_USER_CODE_ADDRESS}")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "Creating a binary file of the <<external_libraries>> section."
        COMMAND ${ARM_OBJCOPY} --output-target=binary --only-section=.section_external_libraries_text --only-section=.section_external_libraries_required_by_the_compiler --only-section=.section_external_libraries_rodata  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_external_libraries_new.bin
        COMMAND cd ${CMAKE_SOURCE_DIR} && ./cmp.sh ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_external_libraries.bin ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_external_libraries_new.bin "${STM32PROG} -c port=${STM32PROG_PORT} freq=${STM32PROG_FREQ} -w ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_external_libraries.bin ${SECTION_EXTERNAL_LIB_ADDRESS}")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "Creating a binary file of the <<user_libraries>> section."
        COMMAND ${ARM_OBJCOPY} --output-target=binary --only-section=.section_user_libraries_text --only-section=.section_user_libraries_required_by_the_compiler --only-section=.section_user_libraries_rodata  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_user_libraries_new.bin
        COMMAND cd ${CMAKE_SOURCE_DIR} && ./cmp.sh ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_user_libraries.bin ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_user_libraries_new.bin "${STM32PROG} -c port=${STM32PROG_PORT} freq=${STM32PROG_FREQ} -w ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_user_libraries.bin ${SECTION_USER_LIBRARIES_ADDRESS}")
endif()