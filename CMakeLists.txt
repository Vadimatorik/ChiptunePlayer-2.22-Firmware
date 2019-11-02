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

SET(MEM_LD "${CMAKE_CURRENT_SOURCE_DIR}/startup/ld/mem.ld")
SET(SECTIONS_LD "${CMAKE_CURRENT_SOURCE_DIR}/startup/ld/sections.ld")

SET(C_COMPILER_FLAGS "-std=gnu99 -fshort-enums")
SET(MC_LD_FLAGS "--specs=nano.specs -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-u,vfprintf -lm -u _printf_float -u _scanf_float -T ${MEM_LD} -T ${SECTIONS_LD} -nostartfiles")

SET(COMPILER_OPTIMIZATION "-O0 -g3")

SET(CMAKE_C_FLAGS "${MC_HARDWARE_FLAGS} ${COMPILER_OPTIMIZATION} ${C_COMPILER_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${MC_HARDWARE_FLAGS} ${COMPILER_OPTIMIZATION} ${MC_LD_FLAGS}")

project(aym C)

file(GLOB SOURCES
        "main.c"

        "startup/src/*.c"

        "lib/st/device/src/*.c"
        "lib/st/hal/src/*.c"
        "lib/freertos/src/*.c"
        "lib/lua/src/*.c"

        "hardware/src/*.c")

include_directories(
        "cfg"

        "startup/inc"

        "lib/st/hal/inc"
        "lib/st/hal/inc/legacy"
        "lib/st/cmsis"
        "lib/st/device/inc"
        "lib/freertos/inc"
        "lib/lua/inc"

        "hardware/inc")

add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_link_options(${PROJECT_NAME}.elf PRIVATE "-Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "${PROJECT_NAME} size information:"
        COMMAND ${ARM_SIZE} ${PROJECT_NAME}.elf -t)