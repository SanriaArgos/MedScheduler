CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    last_name VARCHAR(100) NOT NULL,
    first_name VARCHAR(100) NOT NULL,
    patronymic VARCHAR(100),
    phone VARCHAR(20) NOT NULL UNIQUE,
    user_type VARCHAR(30) NOT NULL DEFAULT 'patient',
    hashed_password TEXT NOT NULL,
    salt TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS hospitals (
    hospital_id SERIAL PRIMARY KEY,
    region TEXT NOT NULL,
    settlement_type TEXT NOT NULL,
    settlement_name TEXT NOT NULL,
    street TEXT NOT NULL,
    house TEXT NOT NULL,
    full_name TEXT NOT NULL,
    administrator_id INT NOT NULL REFERENCES users(id),
    UNIQUE (region, settlement_type, settlement_name, street, house)
);

CREATE TABLE IF NOT EXISTS doctors (
    doctor_id SERIAL PRIMARY KEY,
    user_id INT NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    education TEXT NOT NULL,
    specialty TEXT NOT NULL,
    experience INT NOT NULL,
    hospital_ids INTEGER[]
);

CREATE TABLE IF NOT EXISTS records (
    record_id SERIAL PRIMARY KEY,
    doctor_id INT NOT NULL REFERENCES doctors(doctor_id),
    appointment_date DATE NOT NULL CHECK (EXTRACT(YEAR FROM appointment_date) >= 2025),
    appointment_time TIME NOT NULL CHECK (EXTRACT(HOUR FROM appointment_time) BETWEEN 0 AND 23 AND EXTRACT(MINUTE FROM appointment_time) BETWEEN 0 AND 59),
    hospital_id INT NOT NULL REFERENCES hospitals(hospital_id),
    cabinet_number INT NOT NULL CHECK (cabinet_number >= 1),
    patient_id INT REFERENCES users(id)
);