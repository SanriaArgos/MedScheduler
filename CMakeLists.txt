cmake_minimum_required(VERSION 3.10)
project(medscheduler)

set(CMAKE_CXX_STANDARD 17)

find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(medscheduler
    main.cpp
    database.cpp
    auth.cpp
    registration.cpp
    login.cpp
    handlers.cpp
    system_init.cpp
    user_menu.cpp
    senior_admin_menu.cpp
    junior_admin_menu.cpp
    add_junior_admin.cpp
    add_hospital.cpp
    doctor_hospital_management.cpp
    add_record_slot.cpp
    junior_admin_schedule.cpp
    display_users.cpp
    display_hospitals.cpp
    display_doctors.cpp
    utils.cpp
    doctor_menu.cpp
    doctor_schedule.cpp
    add_doctor.cpp
    patient_schedule.cpp
)

target_include_directories(medscheduler PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(medscheduler PRIVATE ${PostgreSQL_LIBRARIES} OpenSSL::Crypto)