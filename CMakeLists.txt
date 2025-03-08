cmake_minimum_required(VERSION 3.10)
project(MedScheduler)

set(CMAKE_CXX_STANDARD 17)

find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(medscheduler
    main.cpp
    database.cpp
    auth.cpp
)

target_include_directories(medscheduler PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(medscheduler 
    PRIVATE 
    ${PostgreSQL_LIBRARIES}
    OpenSSL::Crypto
)
