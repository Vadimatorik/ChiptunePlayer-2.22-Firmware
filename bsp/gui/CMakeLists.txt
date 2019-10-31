# Минимально необходимая для сборки версия cmake.
CMAKE_MINIMUM_REQUIRED (VERSION 3.13.2)

# Имя библиотеки.
project (USER_BSP_LEVEL)

# Для сборки требуется:
include_directories(
    ${USER_OS_H_FILES_PATH} # Путь до файла user_os с прослойками под конкретную операционную систему.
    ${USER_OS_H_FILES_PATH} # Путь к заголовочным файлам операционной системы.
    ${MODULE_MC_HARDWARE_INTERFACES_H_FILES_PATH}
    ${MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_H_FILES_PATH}
    ${LOW_LEVEL_H_FILES_PATH}
    ${SD_H_FILES_PATH}
    ${USER_FREERTOS_OBJ_H_FILES_PATH}
    ${MODULE_CHIPTUNE_H_FILES_PATH}
    ${MODULE_SHIFT_REGISTER_BASE_H_FILES_PATH}
    ${MODULE_WAVE_GENERATORS_PATH}
    ${MODULE_LCD_DRIVER_PATH}
    ${MODULE_DIGITAL_POTENTIOMETER_PATH}
    ${MC_HARDWARE_PATH}
    ${BUTTON_PATH}
    ${MODULE_FATFS_PATH}
    ${MAKISE_E_PATH}
)

# Компиляции полежат все <<.cpp>> файлы из директории src.
file (GLOB ${PROJECT_NAME}_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/driver/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/fonts/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/callbacks/*.cpp")

# Собираем библиотеку из .obj файлов, полученных из файлов, добавленных выше.
add_library (${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

# Компиляция файлов происходит по флагам из корневого CMakeList.
target_compile_options (${PROJECT_NAME} PRIVATE
                        "${${PROJECT_NAME}_COMPILER_FLAGS}${${PROJECT_NAME}_OPTIMIZATION_FLAGS}")

# Показ флагов компиляции.
add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
                    COMMENT
                    "${PROJECT_NAME} flags: ${${PROJECT_NAME}_COMPILER_FLAGS}${${PROJECT_NAME}_OPTIMIZATION_FLAGS}")

# Вывод размера библиотеки.
add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
                    DEPENDS ${PROJECT_NAME}.a
                    COMMENT "${PROJECT_NAME} size information:"
                    COMMAND cd ${PROJECT_BINARY_DIR}
                    COMMAND ${ARM_SIZE} lib${PROJECT_NAME}.a -t)