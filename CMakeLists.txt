CMAKE_MINIMUM_REQUIRED(VERSION 3.9.6)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# Настройки компилятора.
SET(TOOLCHAIN_BIN_PATH /opt/arm/gcc-arm-none-eabi-7-2018-q2-update/bin)
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_PATH}/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_PATH}/arm-none-eabi-g++)
SET(ARM_SIZE "${TOOLCHAIN_BIN_PATH}/arm-none-eabi-size")
SET(ARM_OBJDUMP "${TOOLCHAIN_BIN_PATH}/arm-none-eabi-objdump")

# Флаги конфигурации аппаратной части (параметры архитектуры).
SET(MICROCONTROLLER_HARDWARE_FLAGS "-mcpu=cortex-m4;-mthumb;-mfloat-abi=hard;-mfpu=fpv4-sp-d16")

PROJECT(ChiptunePlayer-2.22 C CXX ASM)

# Флаги оптимизации библиотек.
SET (FATFS_OPTIMIZATION_FLAGS "-Ofast;-g0;")

# Оставшиеся параметры сборки библиотек.
SET (FATFS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};-std=gnu99;-fshort-enums;")

# Пути к конфигурационным файлам библиотек.
SET (FATFS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

# Сборка требуемых библиотек.
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_fatfs_by_chan)