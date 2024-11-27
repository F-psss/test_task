Разбирался с тем, как в Qt удобно работать с битами, намного дольше чем ожидал
Поиск по директориям рекурсивный

Порядок работы программы:

После запуска настраивается пользователем:
а) маска входных файлов, например .txt, testFile.bin
b) настройка необходимости удалять входные файлы или нет
c) путь для сохранения результирующих файлов
d) действия при повторении имени выходного файла: перезапись или
модификация, например, счетчик к имени файла
e) работа по таймеру или разовый запуск
f) периодичность опроса наличия входного файла (таймер)
g) значение 8 байт для бинарной операции модификации файла
Функциональность: модифицирует входные файлы, например операция XOR с 8-байтной переменной, введенной с формы (Пункт 1,g)
Защита от «дурака»: если входной файл не закрыт - не трогать его.

Среда разработки QT Creator, язык программирования С++, операционная система Windows, компилятор MinGW.