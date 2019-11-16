CMAKE_MINIMUM_REQUIRED(VERSION 3.15.3)

include(CMakeToolchain.cmake)

add_definitions(-D${BUILD_TYPE})

project(aym C)

file(GLOB SOURCES
        "lib/freertos/core/src/*.c"
        "lib/freertos/dummy/src/*.c"
        "lib/lua/src/*.c"
        "lib/u8g2/csrc/*.c"

        "lua/src/*.c"
        "mc_hardware/src/*.c"
        "pcb_hardware/*/src/*.c"
        "logic/aym_psg_parser/src/*.c"
        "startup/src/*.c"

        "main.c")

include_directories(
        "cfg"

        "lib/fatfs/inc"
        "lib/freertos/core/inc"
        "lib/freertos/port/inc"
        "lib/lua/inc"
        "lib/st/hal/inc"
        "lib/st/hal/inc/legacy"
        "lib/st/cmsis"
        "lib/st/device/inc"
        "lib/u8g2/csrc"

        "mc_hardware/inc"

        "pcb_hardware/digital_potentiometer/inc"
        "pcb_hardware/shift_register/inc"
        "pcb_hardware/aym/inc"
        "pcb_hardware/ltc6903/inc"

        "logic/aym_psg_parser/inc"

        "lua/inc"

        "startup/inc")

add_executable(${PROJECT_NAME}.elf ${SOURCES})

if (BUILD_TYPE STREQUAL "AYM_HARDWARE")
    file(GLOB FATFS_SRC
            "lib/fatfs/src/*.c")

    target_sources(${PROJECT_NAME}.elf PRIVATE ${FATFS_SRC})

    target_sources(${PROJECT_NAME}.elf PRIVATE
            "lib/freertos/port/src/freertos_heap_4.c"
            "lib/freertos/port/src/port_cortex_m4.c")

    file(GLOB ST_SRC
            "lib/st/device/src/*.c"
            "lib/st/hal/src/*.c")

    target_sources(${PROJECT_NAME}.elf PRIVATE ${ST_SRC})
endif ()

target_link_options(${PROJECT_NAME}.elf PRIVATE)
target_link_libraries(${PROJECT_NAME}.elf "-lm")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "${PROJECT_NAME} size information:"
        COMMAND ${ARM_SIZE} ${PROJECT_NAME}.elf -t)