ANSWERS
***Answer 29:
1. Возвращаемое значение функции main не int
2. с == a + b - выражение без "эффекта"
3. Нет #include <stdio.h>, поэтому получаются неявные объявления scanf и printf
4. Формат %d в scanf требует аргумент типа int *, но аргумент 2 имеет тип int
5. Переменные a и c неинициализированы в scanf и printf соответственно
***Answer 30:
-rwxr-xr-x 1 s02200450 s02200450 25192 Sep 16 19:06 aplusb
***Answer 31:
26956
***Answer 32:
./aplusb < 001.dat > 001.res
***Answer 33:
cat 001.res
54565
***Answer 34:
0m0.000s
***Answer 35:
560000000
***Answer 36:
Данная команда выведет информацию в длинном формате о процессах, в имени которых присутствует sh
***Answer 37:
ulimit -c выводит ограничение размера создаваемых core dump-ов
ulimit -c 0 делает это ограничение равным нулю
***Answer 38:
Сказали, что решать не надо
***Answer 39:
Для сортировки по размеру файла:
ls -lrS /etc | grep --max-count=5 \\.conf
Для сортировки по длине имени файла:
for str in `ls -lrS /etc`; do NAME_LEN=`expr length $str`; echo $NAME_LEN $str; done | grep -m5 \\.conf | sort -nk1
***Answer 40:
cat /usr/share/dict/words | tr ' ' '\n' | grep -v [aeiou] | wc -w
(stdin)= 3045022100b6e52b6fbce1242c010be19ec01c4358ac03dffea3efcff5085f0252b280cec002203e86ddba3c5b08168d8b474feb05f59479936a6cfae02da3a67b064e618041b3