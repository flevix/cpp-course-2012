Задание №1
=========
В данном задании необходимо написать на ассемблере программу (словарь).
Ей на вход дается набор ключей-значений, разделенных пробелами:
/------------/
 key1 value1
 key2 value2
 key3 value3 
/-----------/
Затем после перевода строки, на каждой строке вводится ключ, для которого программа должна вывести соответствующее ему значение.
Например:
/-------------/
 cat chat
 dog chien
 mouse souris

 dog
 chien
 cat
 chat
/-------------/
Если в качестве ключа передана пустая строка, программа завершается.

В качестве структур данных для словаря следует отсортированный массив, по которому делается бинарный поиск.

Вариант 4.
Сортировка слиянием + бинарный поиск
