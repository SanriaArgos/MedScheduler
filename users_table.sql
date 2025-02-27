DROP TABLE IF EXISTS Users;  -- Удаляем таблицу Users, если она существует
CREATE TABLE Users (         -- Создаем таблицу Users с необходимыми ограничениями
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    last_name TEXT NOT NULL CHECK(LENGTH(last_name) <= 50 AND TRIM(last_name) != ''),
    first_name TEXT NOT NULL CHECK(LENGTH(first_name) <= 50 AND TRIM(first_name) != ''),
    patronymic TEXT NOT NULL CHECK(LENGTH(patronymic) <= 50 AND TRIM(patronymic) != ''),
    role TEXT NOT NULL CHECK(role IN ('пациент','врач','администратор') AND LENGTH(role) <= 50 AND TRIM(role) != ''),
    password_hash TEXT NOT NULL CHECK(LENGTH(password_hash) <= 50 AND TRIM(password_hash) != ''),
    birth_date TEXT NOT NULL CHECK(LENGTH(birth_date) <= 50 AND TRIM(birth_date) != ''),
    email TEXT NOT NULL UNIQUE CHECK(LENGTH(email) <= 50 AND TRIM(email) != ''),
    phone TEXT NOT NULL CHECK(LENGTH(phone) <= 50 AND TRIM(phone) != '')
);
