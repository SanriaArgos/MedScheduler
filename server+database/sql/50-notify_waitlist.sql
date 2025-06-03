CREATE OR REPLACE FUNCTION notify_next_waitlister(p_doctor INT) RETURNS VOID AS $$
DECLARE
  row  RECORD;
  ch   TEXT;
  pl   TEXT;
BEGIN

  SELECT * INTO row
    FROM waitlist
   WHERE doctor_id = p_doctor
     ORDER BY requested_at
   LIMIT 1;

  IF NOT FOUND THEN
    RETURN;
  END IF;


  ch := 'waitlist_user_' || row.patient_id;
  pl := json_build_object(
          'waitlist_id',  row.id,
          'doctor_id',    row.doctor_id,
          'patient_id',   row.patient_id
        )::text;

  PERFORM pg_notify(ch, pl);


  UPDATE waitlist
     SET notified_at = NOW()
   WHERE id = row.id;


  INSERT INTO waitlist_expiry(waitlist_id, expire_at)
       VALUES(row.id, NOW() + INTERVAL '3 hours');
END;
$$ LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION trg_after_cancel_record() RETURNS trigger AS $$
BEGIN
  IF NEW.cancelled = TRUE AND OLD.cancelled = FALSE THEN
    PERFORM notify_next_waitlister(NEW.doctor_id);
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS cancel_notify_trg ON records;
CREATE TRIGGER cancel_notify_trg
  AFTER UPDATE ON records
  FOR EACH ROW EXECUTE FUNCTION trg_after_cancel_record();
