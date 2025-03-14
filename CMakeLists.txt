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
    user_menu.cpp
    utils.cpp
    handlers.cpp
    system_init.cpp
)

target_include_directories(medscheduler PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(medscheduler 
    PRIVATE 
    ${PostgreSQL_LIBRARIES}
    OpenSSL::Crypto
)
