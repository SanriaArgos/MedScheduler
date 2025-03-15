sudo -i -u postgres
psql

\l
\du
\q
exit

psql -U meduser -d medscheduler -f create_tables.sql

sudo -i -u postgres
psql
ALTER ROLE meduser WITH PASSWORD '3671920119';
SELECT rolname, rolpassword FROM pg_authid WHERE rolname = 'meduser';

 psql -U meduser -d medscheduler -h localhost -c "\dt"