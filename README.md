# О проекте
Данный репозиторий содержит в себе исходный код программного обеспечения chiptune-плеера и аналогового синтезатора на базе одного/двух чипов семейства AY (AY-3-8910 или аналоги) и/или YM (YM2149 или аналоги) в 28 и/или 40 выводных DIP корпусах.
# Сборка проекта
Проект может быть собран как под аппаратное обеспечение (основное назначение), так и с целью последующей имитации работы под персональным компьютером  (Linux-приложение).
## Действия перед сборкой
Вне зависимости от типа сборки вам потребуется получить локальную копию проекта со всеми зависимостями. Это можно сделать следующем способом:
```sh
$ git clone git@github.com:Vadimatorik/ChiptunePlayer-2.22-Firmware.git
$ git submodule update --init --recursive
$ cd ChiptunePlayer-2.22-Firmware/c_code/
```
## Сборка проекта под аппаратное обеспечение

## Сборка проекта как Linux-приложение
Для сборки требуется:
* GCC 7.4.0 или новее
* cmake 3.16.2 или новее

Сборку можно произвести следующим способом:
```sh
$ mkdir ../../build_aym_soft_r &&  cd ../../build_aym_soft_r
$ cmake ../ChiptunePlayer-2.22-Firmware/c_code -DCMAKE_BUILD_TYPE=Release -DBUILD_TYPE="AYM_SOFT"
```



Требуется указать путь до компилятора:
-DTOOLCHAIN_BIN_PATH=/opt/arm/gcc-arm-none-eabi-8-2018-q4-major/bin

В случае сборки под железо требуется указать:
-DBUILD_TYPE="AYM_HARDWARE"

В случае сборки под эмуляцию:
-DBUILD_TYPE="AYM_SOFT"
