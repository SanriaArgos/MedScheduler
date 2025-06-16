-- 1. Функция, удаляющая из waitlist записи старше 7 дней
CREATE OR REPLACE FUNCTION cleanup_old_waitlist_entries() RETURNS VOID AS $$
BEGIN
  DELETE
    FROM waitlist
   WHERE requested_at < NOW() - INTERVAL '7 days';
END;
$$ LANGUAGE plpgsql;

-- 2. Планировщик: запускаем функцию ежедневно в 03:00 утра
--    Требуется расширение pg_cron (добавить в shared_preload_libraries).
SELECT cron.schedule(
  'cleanup_waitlist_entries',     -- имя задания
  '0 3 * * *',                    -- каждый день в 03:00
  $$ SELECT cleanup_old_waitlist_entries(); $$
);
