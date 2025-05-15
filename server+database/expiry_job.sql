SELECT cron.schedule(
  'expire_waitlisters',
   '* * * * *',                     
   $$
     WITH expired AS (
       SELECT we.waitlist_id FROM waitlist_expiry we
        LEFT JOIN waitlist w ON w.id = we.waitlist_id
       WHERE NOW() > we.expire_at
         AND we.notified = FALSE
     )
     UPDATE waitlist_expiry SET notified = TRUE
      WHERE waitlist_id IN (SELECT waitlist_id FROM expired);

     DO $$
     DECLARE r RECORD;
     BEGIN
       FOR r IN SELECT * FROM expired LOOP
         DELETE FROM waitlist WHERE id = r.waitlist_id;
         PERFORM notify_next_waitlister(r.doctor_id);
       END LOOP;
     END;
     $$ LANGUAGE plpgsql;
   $$);
