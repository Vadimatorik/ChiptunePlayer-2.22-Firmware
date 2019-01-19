CMAKE_MINIMUM_REQUIRED(VERSION 3.13.2)

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
SET(MICROCONTROLLER_HARDWARE_FLAGS "-mcpu=cortex-m4;-mthumb;-mfloat-abi=hard;-mfpu=fpv4-sp-d16;--specs=nano.specs")

PROJECT(ChiptunePlayer-2.22 C CXX ASM)

SET (C_COMPILER_FLAGS "-std=gnu99;-fshort-enums")
SET (CPP_COMPILER_FLAGS "-Werror;-Wall;-Wextra;-std=c++14;-fno-exceptions;-fshort-enums")

# Путь до файла user_os.h с прослойками под конкретную операционную систему.
SET (USER_OS_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

# Путь к заголовочным файлам операционной системы.
SET (USER_OS_PATH ${CMAKE_SOURCE_DIR}/bsp/submodule/module_freertos_for_stm32f4/inc)

# Флаги оптимизации библиотек.
SET (FATFS_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (FREERTOS_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (SYSTEM_DUMMY_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (STM32F4_LOW_LEVEL_BY_ST_OPTIMIZATION_FLAGS "-Ofast;-g0;")
SET (BUTTONS_OPTIMIZATION_FLAGS "-Ofast;-g0;")

# Оставшиеся параметры сборки библиотек.
SET (FATFS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET (FREERTOS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET (SYSTEM_DUMMY_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
SET (STM32F4_LOW_LEVEL_BY_ST_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${C_COMPILER_FLAGS};")
SET (BUTTONS_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")

# Пути к конфигурационным файлам библиотек.
SET (FATFS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (FREERTOS_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (SYSTEM_DUMMY_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)
SET (STM32F4_LOW_LEVEL_BY_ST_CFG_H_PATH ${CMAKE_SOURCE_DIR}/cfg)

# Сборка требуемых библиотек.
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_fatfs_by_chan)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_freertos_for_stm32f4)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_system_dummy)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_stm32f4_low_level_by_st)
add_subdirectory(${CMAKE_SOURCE_DIR}/bsp/submodule/module_button)
