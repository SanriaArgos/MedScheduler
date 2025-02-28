# Для начала работы с таблицей нужно установить на линукс:

**sqlite3**: ```sudo apt-get install libsqlite3-dev```

**Библиотеку readline**: ```sudo apt-get install libreadline-dev```

**Запустить Cmake и ввести**: ```./MedScheduler```

# Как вывести таблицу на экран? 

**Вводим в терминал поочередно:**

```sqlite3 MedScheduler.db```

```.headers on``` - включает вывод названий колонок;

```.mode column``` - включает режим колонок;

```SELECT * FROM Users;```

