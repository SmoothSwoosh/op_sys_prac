ANSWERS
***Answer 1:
-f
***Answer 2:
1 - для исполняемых программ или команд оболочки
2 - для системных вызовов
***Answer 3:
cat - объединит файлы и распечатает на стандартный вывод
grep - распечатает строки, соответствующие шаблонам
head - выводит первую часть файлов
yes - выводит поданные на вход строки, пока пользователь не завершит работу программы
***Answer 4:
В алфавитном порядке
***Answer 5:
d — файл является каталогом
Могут быть ещё символы: -, c, b, s, p, l
Jul 24 11:41 означает время последней модификации файла
***Answer 6:
-laS
***Answer 7:
host.conf
***Answer 8:
-rw-r--r--. 1 root root    58 Jun 23  2020 /etc/networks
-rw-r--r--  1 root root 27609 Sep 11 20:31 /etc/passwd
***Answer 9:
cat /etc/hosts > ~/file3
cat /etc/passwd >> ~/file3
cat /etc/fstab >> ~/file3
***Answer 10:
rw-r----x 641
r---wx--- 430
rwxr-x--- 750
r--r----x 441
rw-rw--w- 662
***Answer 11:
0022
644
***Answer 12:
umask 26
***Answer 13:
Выполним команду chmod 750 newtask1
drwxr-x--- 2 s02200450 s02200450  4096 Sep 16 13:23 newtask1
***Answer 14:
4 байта
***Answer 15:
После нажатия Enter мы отправили abc в поток ввода, но def - нет, поэтому
в файле будет abc и \n - 4 байта
***Answer 16:
file5 - размер 5 байт (т.к. есть ещё \n)
file6 - размер 4 байта (т.к. нет \n)
(stdin)= 304502206de6526088c8748fe063b1e8ac01d4f92f8cc2dc15bf1e4588dde47cbc8cda92022100918fdfecaac1793ae31d51180f7e2ae44f812c3d638ea4d31abd96d248ae45a5