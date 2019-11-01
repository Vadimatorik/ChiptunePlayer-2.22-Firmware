CMAKE_MINIMUM_REQUIRED(VERSION 3.15.3)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)

SET(TOOLCHAIN_BIN_PATH /opt/arm/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi)
SET(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_PATH}-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_PATH}-g++)
SET(ARM_SIZE "${TOOLCHAIN_BIN_PATH}-size")
SET(ARM_OBJDUMP "${TOOLCHAIN_BIN_PATH}-objdump")
SET(ARM_OBJCOPY "${TOOLCHAIN_BIN_PATH}-objcopy")

SET(MC_HARDWARE_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")

SET(MEM_LD "${CMAKE_CURRENT_SOURCE_DIR}/bsp/ld/mem.ld")
SET(SECTIONS_LD "${CMAKE_CURRENT_SOURCE_DIR}/bsp/ld/sections.ld")

SET(C_COMPILER_FLAGS "-std=gnu99 -fshort-enums")
SET(CPP_COMPILER_FLAGS "-Werror -Wall -Wextra -std=c++14 -Wno-type-limits -fno-exceptions")
SET(MC_LD_FLAGS "--specs=nano.specs -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-u,vfprintf -lm -u _printf_float -u _scanf_float -T ${MEM_LD} -T ${SECTIONS_LD} -Wl,--print-memory-usage -Wl,--undefined=uxTopUsedPriority -Wl,--strip-discarded -Wl,--no-dynamic-linker -nostartfiles")

SET(COMPILER_OPTIMIZATION "-O0 -g3")

set(CMAKE_CXX_FLAGS "${MC_HARDWARE_FLAGS} ${COMPILER_OPTIMIZATION} ${CPP_COMPILER_FLAGS}")
set(CMAKE_C_FLAGS "${MC_HARDWARE_FLAGS} ${COMPILER_OPTIMIZATION} ${C_COMPILER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${MC_HARDWARE_FLAGS} ${COMPILER_OPTIMIZATION} ${MC_LD_FLAGS}")

project(aym C CXX ASM)

file(GLOB SOURCES
        "user_code/ayplayer_class/ayplayer_file/src/*.cpp"
        "user_code/ayplayer_class/base/src/*.cpp"
        "user_code/ayplayer_class/fat/src/*.cpp"
        "user_code/ayplayer_class/gui/elements/src/*.cpp"
        "user_code/ayplayer_class/gui/src/*.cpp"
        "user_code/ayplayer_class/muxer/src/*.cpp"
        "user_code/ayplayer_class/nvic/src/*.cpp"
        "user_code/ayplayer_class/rcc/src/*.cpp"
        "user_code/ayplayer_class/sd_control/src/*.cpp"
        "user_code/ayplayer_obj/*.cpp"
        "user_code/submodules/module_run_time_logger/src/*.cpp"
        "user_code/*.cpp"

        "bsp/freertos/low_level/src/*.cpp"
        "bsp/freertos/user_obj/src/*.cpp"
        "bsp/gui/callbacks/*.cpp"
        "bsp/gui/driver/*.cpp"
        "bsp/gui/fonts/*.cpp"
        "bsp/interrupt/src/*.cpp"
        "bsp/mc_hardware/src/*.cpp"
        "bsp/pcb_hardware/src/*.cpp"
        "bsp/startup/src/*.cpp"
        "bsp/submodules/module_button/buttons_core/src/*.cpp"
        "bsp/submodules/module_chiptune/ay_ym_file_play/src/*.cpp"
        "bsp/submodules/module_chiptune/ay_ym_low_lavel/src/*.cpp"
        "bsp/submodules/module_chiptune/ay_ym_note_mode/src/*.cpp"
        "bsp/submodules/module_digital_potentiometer/ad5204/src/*.cpp"
        "bsp/submodules/module_fat_controller/src/*.cpp"
        "bsp/submodules/module_fatfs_by_chan/src/*.c"
        "bsp/submodules/module_freertos_for_stm32f4/src/*.c"
        "bsp/submodules/module_lcd_driver/st7565/src/*.cpp"
        "bsp/submodules/module_mc_hardware_interfaces_implementation_for_stm32/src/*.cpp"
        "bsp/submodules/module_makise_gui/MakiseGUI/MakiseGUI/*.cpp"
        "bsp/submodules/module_makise_gui/MakiseGUI/MakiseGUI/gui/*.cpp"
        "bsp/submodules/module_makise_gui/MakiseGUI/MakiseGUI/gui/elements/*.cpp"
        "bsp/submodules/module_makise_gui/MakiseGUI/MakiseGUI/gui/elements/src/*.cpp"
        "bsp/submodules/module_makise_gui_elements_by_vadimatorik/src/*.c"
        "bsp/submodules/module_implementation_for_stm32/src/*.cpp"
        "bsp/submodules/module_microsd_low_level_driver/microsd_card_sdio/src/*.cpp"
        "bsp/submodules/shift_register_8bit_port/src/*.cpp"
        "bsp/submodules/shift_register_base/src/*.cpp"
        "bsp/submodules/shift_register_pin/src/*.cpp"
        "bsp/submodules/module_stm32f4_low_level_by_st/cmsis/device/src/*.c"
        "bsp/submodules/module_stm32f4_low_level_by_st/hal/src/*.c"
        "bsp/submodules/module_system_dummy/src/*.cpp"
        "bsp/submodules/module_shift_register/shift_register_8bit_port/src/*.cpp"
        "bsp/submodules/module_shift_register/shift_register_base/src/*.cpp"
        "bsp/submodules/module_shift_register/shift_register_pin/src/*.cpp"
        "bsp/submodules/module_wave_generators/ltc6903/src/*.cpp")

include_directories(
        "cfg"

        "user_code/ayplayer_class/ayplayer_file/inc"
        "user_code/ayplayer_class/base/inc"
        "user_code/ayplayer_class/fat/inc"
        "user_code/ayplayer_class/gui/elements/inc"
        "user_code/ayplayer_class/gui/inc"
        "user_code/ayplayer_class/muxer/inc"
        "user_code/ayplayer_class/nvic/inc"
        "user_code/ayplayer_class/rcc/inc"
        "user_code/ayplayer_class/sd_control/inc"
        "user_code/ayplayer_class/structs"
        "user_code/submodules/module_run_time_logger/inc"

        "bsp/freertos/user_obj/inc"
        "bsp/mc_hardware/inc"
        "bsp/pcb_hardware/inc"
        "bsp/startup/inc"

        "bsp/submodules/module_button/buttons_core/inc"
        "bsp/submodules/module_chiptune/ay_ym_file_play/inc"
        "bsp/submodules/module_chiptune/ay_ym_low_lavel/inc"
        "bsp/submodules/module_chiptune/ay_ym_note_mode/inc"
        "bsp/submodules/module_digital_potentiometer/ad5204/inc"
        "bsp/submodules/module_fat_controller/inc"
        "bsp/submodules/module_fatfs_by_chan/inc"
        "bsp/submodules/module_freertos_for_stm32f4/inc"
        "bsp/submodules/module_lcd_driver"
        "bsp/submodules/module_lcd_driver/st7565/inc"
        "bsp/submodules/module_makise_gui/MakiseGUI/MakiseGUI"
        "bsp/submodules/module_makise_gui/MakiseGUI/MakiseGUI/fonts"
        "bsp/submodules/module_makise_gui/MakiseGUI/MakiseGUI/gui"
        "bsp/submodules/module_makise_gui/MakiseGUI/MakiseGUI/gui/styles"
        "bsp/submodules/module_makise_gui/MakiseGUI/MakiseGUI/gui/elements"
        "bsp/submodules/module_makise_gui/MakiseGUI/MakiseGUI/gui/elements/inc"
        "bsp/submodules/module_makise_gui_elements_by_vadimatorik/inc"
        "bsp/submodules/module_mc_hardware_interfaces"
        "bsp/submodules/module_mc_hardware_interfaces_implementation_for_stm32/inc"
        "bsp/submodules/module_implementation_for_stm32/inc"
        "bsp/submodules/module_microsd_low_level_driver"
        "bsp/submodules/module_microsd_low_level_driver/microsd_card_sdio/inc"
        "bsp/submodules/module_shift_register/shift_register_8bit_port/inc"
        "bsp/submodules/module_shift_register/shift_register_base/inc"
        "bsp/submodules/module_shift_register/shift_register_pin/inc"
        "bsp/submodules/module_stm32f4_low_level_by_st/cmsis/device/inc"
        "bsp/submodules/module_stm32f4_low_level_by_st/cmsis/inc"
        "bsp/submodules/module_stm32f4_low_level_by_st/hal/inc"
        "bsp/submodules/module_stm32f4_low_level_by_st/hal/inc/legacy"
        "bsp/submodules/module_system_dummy/inc"
        "bsp/submodules/module_wave_generators/ltc6903/inc")

add_executable(${PROJECT_NAME}.elf ${SOURCES})