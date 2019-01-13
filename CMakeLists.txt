CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# Toolchain settings
SET(TOOLCHAIN_BIN_PATH /opt/arm/gcc-arm-none-eabi-7-2018-q2-update/bin)
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_PATH}/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_PATH}/arm-none-eabi-g++)
SET(ARM_SIZE "${TOOLCHAIN_BIN_PATH}/arm-none-eabi-size")
SET(ARM_OBJDUMP "${TOOLCHAIN_BIN_PATH}/arm-none-eabi-objdump")

# Compiler settings & flags
SET(OPTIMIZATION_DEBUG_FLAGS "-O0 -g3")
SET(COMMON_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} ${OPTIMIZATION_DEBUG_FLAGS} -std=gnu99 -fshort-enums")
SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} ${OPTIMIZATION_DEBUG_FLAGS} -Wall -Werror -Wextra -std=c++14 -fno-exceptions \
        -fshort-enums -ffunction-sections -fdata-sections")

# Linker settings
SET(LD_FILE "${CMAKE_CURRENT_SOURCE_DIR}/submodule/module_stm32f1_low_level/LD/STM32F103XB_FLASH.ld")
SET(CMAKE_EXE_LINKER_FLAGS_INIT "-fmessage-length=0 -nostartfiles -Xlinker -gc-sections \
        -specs=nosys.specs -specs=nano.specs -fno-exceptions -Wl,--undefined=uxTopUsedPriority -ffunction-sections \
        -Wl,--gc-sections -T ${LD_FILE}")

PROJECT(ChiptunePlayer-2.22 C CXX ASM)

SET(CMAKE_CXX_STANDARD 14)

add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_fatfs_by_chan)