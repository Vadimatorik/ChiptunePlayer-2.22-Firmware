SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)

if (BUILD_TYPE STREQUAL "AYM_HARDWARE")
    if (UNIX)
        SET(CMAKE_C_COMPILER "${TOOLCHAIN_BIN_PATH}arm-none-eabi-gcc")
        SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN_PATH}arm-none-eabi-g++")
        SET(SIZE "${TOOLCHAIN_BIN_PATH}arm-none-eabi-size")
        SET(OBJDUMP "${TOOLCHAIN_BIN_PATH}arm-none-eabi-objdump")
        SET(OBJCOPY "${TOOLCHAIN_BIN_PATH}arm-none-eabi-objcopy")
    elseif (WIN32)
        SET(CMAKE_C_COMPILER "${TOOLCHAIN_BIN_PATH}arm-none-eabi-gcc.exe")
        SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN_PATH}arm-none-eabi-g++.exe")
        SET(SIZE "${TOOLCHAIN_BIN_PATH}arm-none-eabi-size.exe")
        SET(OBJDUMP "${TOOLCHAIN_BIN_PATH}arm-none-eabi-objdump.exe")
        SET(OBJCOPY "${TOOLCHAIN_BIN_PATH}arm-none-eabi-objcopy.exe")
    endif ()
elseif (BUILD_TYPE STREQUAL "AYM_SOFT")
    if (UNIX)
        SET(CMAKE_C_COMPILER "gcc")
        SET(CMAKE_CXX_COMPILER "g++")
        SET(SIZE "size")
        SET(OBJDUMP "objdump")
        SET(OBJCOPY "objcopy")
    elseif (WIN32)
        SET(CMAKE_C_COMPILER "gcc.exe")
        SET(CMAKE_CXX_COMPILER "g++.exe")
        SET(SIZE "size.exe")
        SET(OBJDUMP "objdump.exe")
        SET(OBJCOPY "objcopy.exe")
    endif ()
else ()
    message(FATAL_ERROR "Variable BUILD TYPE is not set!")
endif ()

SET(C_COMPILER_FLAGS "-std=gnu99")
SET(COMPILER_OPTIMIZATION "-O0 -g3")

if (BUILD_TYPE STREQUAL "AYM_HARDWARE")
    SET(MC_HARDWARE_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")

    SET(MEM_LD "${CMAKE_CURRENT_SOURCE_DIR}/startup/ld/mem.ld")
    SET(SECTIONS_LD "${CMAKE_CURRENT_SOURCE_DIR}/startup/ld/sections.ld")

    SET(MC_LD_FLAGS "--specs=nano.specs -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-u,vfprintf -lm -u _printf_float -u _scanf_float -T ${MEM_LD} -T ${SECTIONS_LD} -nostartfiles")

    SET(CMAKE_C_FLAGS "${MC_HARDWARE_FLAGS} -fdata-sections -ffunction-sections -Wl,--gc-sections ${COMPILER_OPTIMIZATION} ${C_COMPILER_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${MC_HARDWARE_FLAGS} -fdata-sections -ffunction-sections -Wl,--gc-sections ${COMPILER_OPTIMIZATION} ${MC_LD_FLAGS}")
elseif (BUILD_TYPE STREQUAL "AYM_SOFT")
    SET(CMAKE_C_FLAGS "${COMPILER_OPTIMIZATION} -fdata-sections -ffunction-sections -Wl,--gc-sections  ${C_COMPILER_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "-fdata-sections -ffunction-sections -Wl,--gc-sections ${COMPILER_OPTIMIZATION}")
    option(TRACE_FUNCTIONS "Trace function calls using instrument-functions")
    if(TRACE_FUNCTIONS)
        add_definitions(-DTRACE_FUNCTIONS)
        SET(GCC_COVERAGE_COMPILE_FLAGS "-finstrument-functions")
        target_compile_options(FreeRTOS_Emulator PUBLIC ${GCC_COVERAGE_COMPILE_FLAGS})
    endif(TRACE_FUNCTIONS)
endif ()
