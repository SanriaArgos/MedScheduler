# Инструкция по установке и запуску БД MedScheduler

Следуйте этой инструкции, чтобы установить необходимые библиотеки, настроить PostgreSQL, собрать и запустить приложение.

---

## 1. Установка необходимых библиотек на Linux/ WSL/ Ubuntu

Обновите список пакетов в терминале:
```bash
sudo apt update
```

Установите необходимые пакеты:
```bash
sudo apt install build-essential libpq-dev libssl-dev postgresql postgresql-contrib
```
* **build-essential** – включает компиляторы и make.
    
* **libpq-dev** – библиотеки для работы с PostgreSQL.
    
* **libssl-dev** – библиотеки OpenSSL для хеширования паролей.
    
* **postgresql** и **postgresql-contrib** – сервер PostgreSQL и дополнительные инструменты.

---
## 2. Настройка PostgreSQL

**2.1 Cоздание пользователя и БД**

  Переключитесь на пользователя PostgreSQL:
  ```
  sudo -i -u postgres
  ```
  
  Запустите интерактивную консоль ```psql```:
  ```
  psql
  ```
  
  Создайте нового пользователя (например, meduser) с паролем:
  ```
  CREATE USER meduser WITH PASSWORD 'ваш пароль';
  ```
  
  Создайте базу данных (например, medscheduler) и назначьте владельцем созданного пользователя:
  ```
  CREATE DATABASE medscheduler OWNER meduser;
  ```
  
  Выйдите из ```psql``` (введите ```\q```) и переключитесь обратно на обычного пользователя:
  ```
  exit
  ```

**2.2 Импорт и просмотр таблиц**

Теперь выполните команду для импорта схемы таблиц в базу данных:
```
psql -U meduser -d medscheduler -f create_tables.sql
```

* Для просмотра списка импортированных таблиц выполните команду:
  ```
  psql -U meduser -d medscheduler -h localhost -c "\dt"
  ```

* А для просмотра данных конкретной таблицы нужно, опять же, запустить консоль:
  ```
  psql -U meduser -d medscheduler -h localhost
  ```
  
  И, введя пароль, передать команду ```SELECT * FROM *название таблицы*;```

---
# 3. Как открыть контейнер базы данных в Docker?
```
docker exec -it serverdatabase-db-1 psql -U meduser -d medscheduler
```
Запускать команду из папки server+database

# 4. Как снести базу данных в контейнере?
```
docker system prune --volumes 
docker-compose down -v
```

Для сборки контейнера используем привычную команду:
```
docker-compose up --build
```
