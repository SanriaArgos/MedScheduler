-- Создаем пользователя meduser и даем ему права на базу данных medscheduler
CREATE USER meduser WITH PASSWORD '3671920119';
GRANT ALL PRIVILEGES ON DATABASE medscheduler TO meduser;
ALTER DATABASE medscheduler OWNER TO meduser;

-- Если таблицы создаются пользователем postgres (по умолчанию для initdb.d скриптов),
-- и meduser должен иметь к ним доступ, смена владельца БД (ALTER DATABASE ... OWNER TO meduser)
-- обычно решает эту проблему.
-- Дополнительно, для явного предоставления прав на схему public:
GRANT ALL ON SCHEMA public TO meduser;

