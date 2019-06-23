#!/bin/bash

#
# $1 - старый файл. Он может отсутствовать, если происходит первая компиляция.
# $2 - только что извлеченный из elf файл.
# $3 - строка для прошивки требуемого сектора STM32.
#

echo "Old file name: $1" 
echo "New file name: $2" 

# Предполагаем, что ничего перепрошивать не придется.
flag_rewrite=0

# Если это первая компиляция, то старого файла не существует и можно просто
# переименовать новый, чтобы он назывался как старый (потому что прошивка
# происходит по имени старого файла).
# Если это новая компиляция, то нужно узнать, различаются ли чем-то файлы или нет
# если да, то надо заменить старый новым. Если нет, то удалить новый.
if [ -e $1 ]
then # Если это уже не первая компиляция и файл существовал.
echo "Both files exist."
# Получаем md5 сумму входных файлов, анализируя файл в бинарном формате.
buf=$(md5sum $1 --binary)
md5_old=${buf:0:32} # Из вывода нам нужна только md5 без имени флага. 
                    # Это первые 32 символа.
buf=$(md5sum $2 --binary)
md5_new=${buf:0:32}

echo "Started file comparison." 
if [ $md5_old == $md5_new ]
then # Если файл не был обновлен, удаляем новый.
echo "The file has not been updated."
echo "The new file will be deleted."
rm $2
echo "Removed."
else # Если они разные, тогда заменяем старый новым.
echo "The file has been modified."
echo "Old will be replaced by new."
mv $2 $1
echo "Replaced."
flag_rewrite=1 # Придется перепрошивать этот сектор.
fi
else # Если это первая компиляция.
echo "Old file does not exist."
echo "New will be renamed to old."
mv $2 $1 # Переименовываем файл под старый.
flag_rewrite=1 # Придется перепрошивать этот сектор.
echo "Renamed."
fi

# Если файл был обновлен или только появился, то им надо прошить контроллер.
if [ $flag_rewrite -eq 1 ]
then
echo "Started flashing."
echo "CMD params: $3"
$3
fi



