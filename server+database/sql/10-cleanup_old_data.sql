CREATE OR REPLACE FUNCTION clean_old_records() RETURNS VOID LANGUAGE plpgsql AS $$
BEGIN
  DELETE FROM records
   WHERE appointment_date < (CURRENT_DATE - INTERVAL '1 year');
END;
$$;

CREATE OR REPLACE FUNCTION purge_inactive_patients() RETURNS VOID LANGUAGE plpgsql AS $$
BEGIN
  DELETE FROM users u
   WHERE u.user_type = 'patient'
     AND u.last_login < (CURRENT_DATE - INTERVAL '5 years');
END;
$$;

SELECT cron.schedule(
  'daily_clean_records',
  '0 0 * * *',
  'SELECT clean_old_records();'
);

SELECT cron.schedule(
  'daily_purge_patients',
  '0 1 * * *',
  'SELECT purge_inactive_patients();'
);