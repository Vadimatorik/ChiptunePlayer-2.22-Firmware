CMAKE_MINIMUM_REQUIRED(VERSION 3.15.3)

project(aym)

include(CMakeToolchain.cmake)

add_definitions(-D${BUILD_TYPE})

file(GLOB SOURCES
        "lib/fatfs/src/*.c"
        "lib/lua/src/*.c"
        "lib/u8g2/csrc/*.c"

        "bsp/mc_hardware/src/*.c"
        "bsp/pcb_hardware/*/src/*.c"

        "logic/lua_lib/src/*.c"
        "logic/aym_psg_parser/src/*.c"

        "lib/freertos/dummy/src/*.c"
        "lib/freertos/core/src/*.c"

        "main.c"
        )

include_directories(
        "cfg"

        "lib/fatfs/inc"
        "lib/lua/inc"
        "lib/st/hal/inc"
        "lib/st/hal/inc/legacy"
        "lib/st/cmsis"
        "lib/st/device/inc"
        "lib/u8g2/csrc"

        "bsp/mc_hardware/inc"

        "bsp/pcb_hardware/digital_potentiometer/inc"
        "bsp/pcb_hardware/shift_register/inc"
        "bsp/pcb_hardware/aym/inc"
        "bsp/pcb_hardware/ltc6903/inc"
        "bsp/pcb_hardware/lcd/inc"

        "logic/aym_psg_parser/inc"

        "lib/freertos/core/inc"

        "logic/lua_lib/inc"

        "bsp/mc_hardware/startup/inc"

        "resurse/img")

if (BUILD_TYPE STREQUAL "AYM_HARDWARE")
    SET(OUT_FILE "${PROJECT_NAME}.elf")
elseif (BUILD_TYPE STREQUAL "AYM_SOFT")
    SET(OUT_FILE "${PROJECT_NAME}")
endif ()

add_executable(${OUT_FILE} ${SOURCES})

if (BUILD_TYPE STREQUAL "AYM_HARDWARE")
    target_include_directories(${OUT_FILE} PUBLIC
            "lib/freertos/port/mc/inc"
            )

    file(GLOB BUILD_MODE_SRC_SRC
            "lib/st/device/src/*.c"
            "lib/st/hal/src/*.c"
            "lib/freertos/port/mc/src/*.c"
            "bsp/mc_hardware/startup/src/*.c"
            )

elseif (BUILD_TYPE STREQUAL "AYM_SOFT")
    find_package(Threads)

    target_include_directories(${OUT_FILE} PUBLIC
            "lib/freertos/port/posix/inc"
            "bsp/socket_emul_layer/inc")

    file(GLOB BUILD_MODE_SRC_SRC
            "lib/freertos/port/posix/src/*.c"
            "bsp/socket_emul_layer/src/*.c"
            )

    target_link_libraries(${OUT_FILE} ${CMAKE_THREAD_LIBS_INIT})

    add_definitions(-DLUA_USE_LINUX)
    target_link_libraries(${OUT_FILE} dl)
endif ()

target_sources(${OUT_FILE} PRIVATE ${BUILD_MODE_SRC_SRC})
target_link_libraries(${OUT_FILE} "-lm")

add_custom_command(TARGET ${OUT_FILE} POST_BUILD
        DEPENDS ${OUT_FILE}
        COMMENT "${PROJECT_NAME} size information:"
        COMMAND ${SIZE} ${OUT_FILE} -t)
