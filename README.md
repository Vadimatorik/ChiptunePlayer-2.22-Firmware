# О проекте
Данный репозиторий содержит в себе исходный код программного обеспечения chiptune-плеера и аналогового синтезатора на базе одного/двух чипов семейства AY (AY-3-8910 или аналоги) и/или YM (YM2149 или аналоги) в 28 и/или 40 выводных DIP корпусах.
# Сборка и запуск
Проект может быть собран как под аппаратное обеспечение (основное назначение), так и с целью последующей имитации работы под персональным компьютером  (Linux-приложение).
## Действия перед сборкой
Вне зависимости от типа сборки вам потребуется получить локальную копию проекта со всеми зависимостями. Это можно сделать следующем способом:
```sh
$ git clone git@github.com:Vadimatorik/ChiptunePlayer-2.22-Firmware.git
$ cd ChiptunePlayer-2.22-Firmware
$ git checkout dev
$ git submodule update --init --recursive
```

## Сборка проекта под аппаратное обеспечение
Для сборки требуется:
* gcc-arm-none-eabi-9-2019-q4-major или новее
* cmake 3.16.2 или новее

Сборку можно произвести следующим способом (действия выполнять из корневого каталога проекта):
```sh
$ cd .. && mkdir build_aym_hard_r &&  cd build_aym_hard_r
$ cmake ../ChiptunePlayer-2.22-Firmware/c_code -DCMAKE_BUILD_TYPE=HARD -DTOOLCHAIN_BIN_PATH=/opt/arm/gcc-arm-none-eabi-8-2018-q4-major/bin/
$ make all -j
```

## Прошивка микроконтроллера
Перед тем, как прошивать микроконтроллер убедитесь, что:
* Питание на микроконтроллер подано (кнопка подачи питания на время отладки может быть закорочена)
* Программатор верно подключен к плате
* Установлена программа stm32cubeprogrammer

Прошить микроконтроллер средствами утилиты st-flash программатором st-link v2 можно введя следующие команды:
```sh
$ STM32_Programmer_CLI -c port="SWD" -e all -w aym.elf -hardRst
```

## Подготовка карты памяти (micro-sd)
В корне карты памяти должна находиться копия директории lua_scripts из корня проекта.

## Сборка проекта как Linux-приложение
Для сборки требуется:
* GCC 7.4.0 или новее
* cmake 3.16.2 или новее

Сборку можно произвести следующим способом (действия выполнять из корневого каталога проекта):
```sh
$ cd .. && mkdir build_aym_soft_r &&  cd build_aym_soft_r
$ cmake ../ChiptunePlayer-2.22-Firmware/c_code -DCMAKE_BUILD_TYPE="SOFT" 
$ make all -j
$ cd ..
```

### Сборки сопутствующих утилит
Для работы так же потребуются следующие сопутствующие утилиты:
* keyboarde
* lcde

Для их получения и сборки потребуется выполнить следующие действия:
```sh
$ git clone git@github.com:Vadimatorik/keyboarde.git
$ mkdir build_keyboarde_r &&  cd build_keyboarde_r
$ cmake ../keyboarde -DCMAKE_BUILD_TYPE=Release
$ make -j
$ cd ..
$ git clone git@github.com:Vadimatorik/lcde.git
$ mkdir build_lcde_r &&  cd build_lcde_r
$ cmake ../lcde -DCMAKE_BUILD_TYPE=Release
$ make -j
$ cd ..
```
### Создание образа карты памяти
Так же потребуется образ microsd карты, на которой будут Lua-скрипты и файлы пользователя. Для его создания и подключения как съемного диска следует сделать следующие действия:
```sh
$ cd ChiptunePlayer-2.22-Firmware
$ mkdir resurse && cd resurse && mkdir microsd
$ sudo dd if=/dev/zero of=microsd.img bs=256M count=1
$ sudo mkfs.fat -F32 -v -I microsd.img
$ sudo chmod 777 microsd.img
$ sudo mount -t auto microsd.img microsd
```

После этого на карту можно положить требуемые файлы (файлы треков).
Для закрытия карты следует сделать следующее:
```sh
$ sudo umount microsd
$ cd ../..
```

### Запуск проекта как Linux-приложения
Для запуска приложения в корне проекта с исходниками имеется скрипт, который запускает необходимые для имитации сопутствующие утилиты, монтирует карту памяти, копирует на нее актуальные Lua-скрипты, отключает карту памяти, запускает приложение.
Для его запуска введите следующее:
```sh
$ cd ChiptunePlayer-2.22-Firmware
$ ./start_full_sm.sh
```
