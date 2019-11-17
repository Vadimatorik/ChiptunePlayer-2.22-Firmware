# ChiptunePlayer-2.22-MainBoard-v2-Firmware

Требуется указать путь до компилятора:
-DTOOLCHAIN_BIN_PATH=/opt/arm/gcc-arm-none-eabi-8-2018-q4-major/bin

В случае сборки под железо требуется указать:
-DBUILD_TYPE="AYM_HARDWARE"

В случае сборки под эмуляцию:
-DBUILD_TYPE="AYM_SOFT"