# Wordle на языке C

Это реализация популярной игры Wordle на языке программирования C. В этой версии игры используются английские слова, загружаемые из файла `dictionary.txt`.

## Описание

Wordle - это игра, в которой у вас есть 6 попыток, чтобы угадать загаданное слово. После каждой попытки вам даются подсказки о том, какие буквы находятся на правильных местах, какие буквы есть в слове, но находятся на неправильных местах, а какие буквы отсутствуют в загаданном слове.

Подсказки по символам:
- `_` - Этот символ вообще не существует в слове.
- `o` - Этот символ существует в слове, но находится не на своем месте.
- `#` - Этот символ правильный.

Приложение также учитывает наличие двойных букв. Например, если вы угадываете слово ZORRO, когда загаданное слово CRANE, оно покажет подсказку `__o__`, а не `__oo_`.

## Установка

1. Склонируйте репозиторий на ваш локальный компьютер
2. Поместите файл `dictionary.txt` рядом с исполняемым файлом
