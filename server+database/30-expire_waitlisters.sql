SELECT cron.schedule(
  'expire_waitlisters',
  '* * * * *',
  $$
    -- Обновляем статус устаревших записей
    UPDATE waitlist_expiry SET notified = TRUE
    WHERE waitlist_id IN (
      SELECT we.waitlist_id 
      FROM waitlist_expiry we
      LEFT JOIN waitlist w ON w.id = we.waitlist_id
      WHERE NOW() > we.expire_at
        AND we.notified = FALSE
    );

    -- Удаляем устаревшие записи и уведомляем следующих
    DELETE FROM waitlist
    WHERE id IN (
      SELECT we.waitlist_id 
      FROM waitlist_expiry we
      WHERE NOW() > we.expire_at
        AND notified = TRUE
    );

    -- Уведомляем следующих в очереди (если функция существует)
    PERFORM (
      SELECT notify_next_waitlister(doctor_id)
      FROM waitlist_expiry we
      JOIN waitlist w ON w.id = we.waitlist_id
      WHERE NOW() > we.expire_at
        AND we.notified = TRUE
      LIMIT 1
    );
  $$
);