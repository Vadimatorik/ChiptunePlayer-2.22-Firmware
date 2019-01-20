# Минимально необходимая для сборки версия cmake.
CMAKE_MINIMUM_REQUIRED (VERSION 3.13.2)

# Имя библиотеки.
project (USER_CODE)

# Для сборки требуется:
include_directories(
    ${USER_OS_H_PATH} # Путь до файла user_os с прослойками под конкретную операционную систему.
    ${USER_OS_PATH} # Путь к заголовочным файлам операционной системы.
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${MODULE_MC_HARDWARE_INTERFACES_PATH}
    ${MODULE_MC_HARDWARE_INTERFACES_IMPLEMENTATION_PATH}
    ${LOW_LEVEL_H_FILES_PATH}
    ${SD_H_PATH}
    ${USER_FREERTOS_OBJ_H_PATH}
    ${CHIPTUNE_H_PATH}
    ${SHIFT_REGISTER_BASE_PATH}
    ${WAVE_GENERATORS_PATH}
    ${LCD_DRIVER_PATH}
    ${DIGITAL_POTENTIOMETER_PATH}
    ${MC_HARDWARE_PATH}
    ${BUTTON_PATH}
    ${FATFS_PATH}
    ${PCB_HARDWARE_PATH}
    ${MAKISE_E_PATH}
    ${MAKISE_GUI_ELEMENTS_BY_VADIMATORIK_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/ayplayer_file/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/base/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/fat/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/gui/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/gui/elements/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/muxer/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/nvic/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/rcc/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/sd_control/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/structs
    ${CMAKE_CURRENT_SOURCE_DIR}/submodule/module_run_time_logger/inc
)

# Компиляции полежат все <<.cpp>> файлы из директории src.
file (GLOB ${PROJECT_NAME}_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/ayplayer_file/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/base/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/fat/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/gui/elements/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/gui/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/muxer/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/nvic/src/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_class/sd_control/*.cpp"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/ayplayer_obj/*.cpp")

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