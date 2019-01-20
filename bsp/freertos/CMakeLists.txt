# Минимально необходимая для сборки версия cmake.
CMAKE_MINIMUM_REQUIRED (VERSION 3.13.2)

# Имя библиотеки.
project (USER_FREERTOS_LEVEL)

# Производится проверка на то, что параметры оптимизации
# проекта (библиотеки) заданы.
# Они должны быть заданы в корневом проекте.
# Пример:
# SET (USER_FREERTOS_LEVEL_OPTIMIZATION_FLAGS "-O0;-g3;")
if (NOT DEFINED ${PROJECT_NAME}_OPTIMIZATION_FLAGS)
    message (FATAL_ERROR
             "The value of the variable <<${PROJECT_NAME}_OPTIMIZATION_FLAGS>> is not specified!")
endif ()

# Производится проверка на то, что параметры аппаратной
# части для компилляции проекта (библиотеки) заданы.
# Они должны быть заданы в корневом проекте.
# Параметры должны быть обязательно разделены через <<;>>.
# Пример:
# SET (USER_FREERTOS_LEVEL_COMPILER_FLAGS "${MICROCONTROLLER_HARDWARE_FLAGS};${CPP_COMPILER_FLAGS};")
if (NOT DEFINED ${PROJECT_NAME}_COMPILER_FLAGS)
    message (FATAL_ERROR
             "The value of the variable <<${PROJECT_NAME}_COMPILER_FLAGS>> is not specified!")
endif ()

# Путь до файла user_os.h с прослойками под конкретную операционную систему.
# Должен быть обязательно указан.
if (NOT DEFINED USER_OS_H_PATH)
    message (FATAL_ERROR
            "The value of the variable <<USER_OS_H_PATH>> is not specified!")
endif()

# Путь к заголовочным файлам операционной системы.
if (NOT DEFINED USER_OS_PATH)
    message (FATAL_ERROR
            "The value of the variable <<USER_OS_PATH>> is not specified!")
endif()

# Путь до submodule с описанием интерфейсов должен быть указан обязательно.
# Пример:
# SET (MODULE_USER_FREERTOS_LEVEL_INTERFACES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodule/module_mc_hardware_interfaces)
if (NOT DEFINED MODULE_MC_HARDWARE_INTERFACES_PATH)
    message (FATAL_ERROR
            "The value of the variable <<MODULE_MC_HARDWARE_INTERFACES_PATH>> is not specified!")
endif()

# Путь к реализациям интерфейсов под текущий микроконтроллер.
if (NOT DEFINED MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_PATH)
    message (FATAL_ERROR
            "The value of the variable <<MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_PATH>> is not specified!")
endif()

# Путь до заголовочных файлов работы с аппаратной периферией низкого уровня.
# Пример:
# SET (LOW_LEVEL_H_FILES_PATH ${CMAKE_SOURCE_DIR}/bsp/submodule/module_stm32f4_low_level_by_st/inc)
if (NOT DEFINED LOW_LEVEL_H_FILES_PATH)
    message (FATAL_ERROR
            "The value of the variable <<LOW_LEVEL_H_FILES_PATH>> is not specified!")
endif()

# Нужны структуры плеера.
if (NOT DEFINED CHIPTUNE_H_PATCH)
    message (FATAL_ERROR
            "The value of the variable <<CHIPTUNE_H_PATCH>> is not specified!")
endif()

# Плеер за собой тянет сдвиговые регистры.
if (NOT DEFINED SHIFT_REGISTER_BASE_PATH)
    message (FATAL_ERROR
            "The value of the variable <<SHIFT_REGISTER_BASE_PATH>> is not specified!")
endif()


# Для сборки требуется:
include_directories(
    ${USER_OS_H_PATH} # Путь до файла user_os с прослойками под конкретную операционную систему.
    ${USER_OS_PATH} # Путь к заголовочным файлам операционной системы.
    ${MODULE_MC_HARDWARE_INTERFACES_PATH}
    ${MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_PATH}
    ${LOW_LEVEL_H_FILES_PATH}
    ${CHIPTUNE_H_PATCH}
    ${SHIFT_REGISTER_BASE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/low_level/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/user_obj/inc
)

# Компиляции полежат все <<.cpp>> файлы из директории src.
file (GLOB ${PROJECT_NAME}_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/low_level/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/user_obj/src/*.cpp")

# Собираем библиотеку из .obj файлов, полученных из файлов, добавленных выше.
add_library (${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

# Компиляция файлов происходит по флагам из корневого CMakeList.
target_compile_options (${PROJECT_NAME} PRIVATE
                        "${${PROJECT_NAME}_COMPILER_FLAGS}${${PROJECT_NAME}_OPTIMIZATION_FLAGS}")

# Показ флагов компиляции.
add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
                    COMMENT
                    "${PROJECT_NAME} flags: ${${PROJECT_NAME}_COMPILER_FLAGS}${${PROJECT_NAME}_OPTIMIZATION_FLAGS}")

# Вывод коммита.
add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
                    COMMENT "${PROJECT_NAME} commit:"
                    COMMAND git rev-parse HEAD)

# Вывод размера библиотеки.
add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
                    DEPENDS ${PROJECT_NAME}.a
                    COMMENT "${PROJECT_NAME} size information:"
                    COMMAND cd ${PROJECT_BINARY_DIR}
                    COMMAND ${ARM_SIZE} lib${PROJECT_NAME}.a -t)