CREATE OR REPLACE FUNCTION schedule_appointment_notification() 
  RETURNS TRIGGER LANGUAGE plpgsql AS $$
BEGIN
  IF NEW.patient_id IS NOT NULL THEN
    INSERT INTO appointment_notifications(record_id, user_id, send_time, payload)
    SELECT
      NEW.record_id,
      NEW.patient_id,
      (NEW.appointment_date + NEW.appointment_time) - INTERVAL '24 hours',
      jsonb_build_object(
        'doctor_name',        du.last_name || ' ' || du.first_name || COALESCE(' '||du.patronymic,''),
        'specialty',          d.specialty,
        'hospital_address',   h.region||', '||h.settlement_type||' '||h.settlement_name||', '||h.street||' '||h.house,
        'junior_admin_phone', ja.phone,
        'cabinet_number',     NEW.cabinet_number,
        'appointment_time',   to_char(NEW.appointment_date + NEW.appointment_time, 'YYYY-MM-DD HH24:MI')
      )
    FROM doctors d
    JOIN users du ON d.user_id = du.id
    JOIN hospitals h ON NEW.hospital_id = h.hospital_id
    JOIN users ja ON h.administrator_id = ja.id
    WHERE d.doctor_id = NEW.doctor_id;
  END IF;
  RETURN NEW;
END;
$$;

DROP TRIGGER IF EXISTS trg_schedule_notification ON records;
CREATE TRIGGER trg_schedule_notification
  AFTER INSERT ON records
  FOR EACH ROW EXECUTE FUNCTION schedule_appointment_notification();

CREATE OR REPLACE FUNCTION send_due_appointment_notifications() RETURNS void LANGUAGE plpgsql AS $$
DECLARE
  rec RECORD;
BEGIN
  FOR rec IN 
    SELECT id, payload
      FROM appointment_notifications
     WHERE send_time <= NOW() AND NOT sent
  LOOP
    PERFORM pg_notify('appointment_reminder', rec.payload::text);
    UPDATE appointment_notifications 
       SET sent = TRUE 
     WHERE id = rec.id;
  END LOOP;
END;
$$;

SELECT cron.schedule(
  'send_appointment_reminders',
  '*/1 * * * *',
  $$SELECT send_due_appointment_notifications();$$
);