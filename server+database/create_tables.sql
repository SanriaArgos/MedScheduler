-- Удаление существующих таблиц, если они уже существуют
DROP TABLE IF EXISTS records;
DROP TABLE IF EXISTS hospitals;
DROP TABLE IF EXISTS users;

-- Создание таблицы пользователей
CREATE TABLE users (
    id SERIAL PRIMARY KEY,                   -- Уникальный идентификатор пользователя
    last_name VARCHAR(100) NOT NULL,           -- Фамилия (обязательное поле)
    first_name VARCHAR(100) NOT NULL,          -- Имя (обязательное поле)
    patronymic VARCHAR(100),                   -- Отчество (необязательное поле)
    phone VARCHAR(20) NOT NULL UNIQUE,         -- Номер телефона (обязательное поле, должен быть уникальным)
    user_type VARCHAR(40) NOT NULL DEFAULT 'patient',  -- Тип пользователя (по умолчанию 'patient')
    hashed_password TEXT NOT NULL,             -- Хеш пароля
    salt TEXT NOT NULL                         -- Соль для хеширования пароля
);

-- Создание таблицы больниц
CREATE TABLE hospitals (
    hospital_id SERIAL PRIMARY KEY,            -- Уникальный идентификатор больницы
    address TEXT NOT NULL UNIQUE,              -- Адрес больницы
    administrator_id INT NOT NULL REFERENCES users(id)  -- Идентификатор администратора
);

-- Создание таблицы записей на приём
CREATE TABLE records (
    record_id SERIAL PRIMARY KEY,              -- Уникальный идентификатор записи
    patient_id INT NOT NULL REFERENCES users(id),    -- Идентификатор пациента
    doctor_id INT NOT NULL REFERENCES users(id),     -- Идентификатор врача
    hospital_id INT NOT NULL REFERENCES hospitals(hospital_id), -- Идентификатор больницы
    time_and_date TIMESTAMP NOT NULL            -- Дата и время приёма
);
