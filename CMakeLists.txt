CMAKE_MINIMUM_REQUIRED(VERSION 3.15.3)

project(aym)

include(CMakeToolchain.cmake)

add_definitions(-D${BUILD_TYPE})
add_definitions(-DLUA_USERCONFIG="lua/inc/l")

file(GLOB SOURCES
        "lib/lua/src/*.c"
        "lib/u8g2/csrc/*.c"

        "lua/src/*.c"
        "mc_hardware/src/*.c"
        "pcb_hardware/*/src/*.c"
        "logic/aym_psg_parser/src/*.c"

        "lib/freertos/dummy/src/*.c"
        "lib/freertos/core/src/*.c"

        "main.c"
        )

include_directories(
        "cfg"

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
        "pcb_hardware/lcd/inc"

        "logic/aym_psg_parser/inc"

        "lib/freertos/core/inc"

        "lua/inc"

        "startup/inc")

if (BUILD_TYPE STREQUAL "AYM_HARDWARE")
    SET(OUT_FILE "${PROJECT_NAME}.elf")
elseif (BUILD_TYPE STREQUAL "AYM_SOFT")
    SET(OUT_FILE "${PROJECT_NAME}")
endif ()

add_executable(${OUT_FILE} ${SOURCES})

if (BUILD_TYPE STREQUAL "AYM_HARDWARE")
    target_include_directories(${OUT_FILE} PUBLIC
            "lib/freertos/port/mc/inc"
            "lib/fatfs/inc"
            )

    file(GLOB BUILD_MODE_SRC_SRC
            "lib/st/device/src/*.c"
            "lib/st/hal/src/*.c"
            "startup/src/*.c"
            "lib/freertos/port/mc/src/*.c"
            "lib/fatfs/src/*.c")
elseif (BUILD_TYPE STREQUAL "AYM_SOFT")
    find_package(Threads)

    target_include_directories(${OUT_FILE} PUBLIC
            "lib/freertos/port/posix/inc")

    file(GLOB BUILD_MODE_SRC_SRC
            "lib/freertos/port/posix/src/*.c"
            )

    target_link_libraries(${OUT_FILE} ${CMAKE_THREAD_LIBS_INIT})
endif ()

target_sources(${OUT_FILE} PRIVATE ${BUILD_MODE_SRC_SRC})
target_link_libraries(${OUT_FILE} "-lm")

add_custom_command(TARGET ${OUT_FILE} POST_BUILD
        DEPENDS ${OUT_FILE}
        COMMENT "${PROJECT_NAME} size information:"
        COMMAND ${SIZE} ${OUT_FILE} -t)
