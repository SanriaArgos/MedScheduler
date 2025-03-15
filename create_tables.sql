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

CREATE TABLE IF NOT EXISTS records (
    record_id SERIAL PRIMARY KEY,
    patient_id INT NOT NULL REFERENCES users(id),
    doctor_id INT NOT NULL REFERENCES users(id),
    hospital_id INT NOT NULL REFERENCES hospitals(hospital_id),
    time_and_date TIMESTAMP NOT NULL
);
