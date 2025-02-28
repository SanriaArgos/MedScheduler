cmake_minimum_required(VERSION 3.10)
project(MedScheduler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/SQLite)
include_directories(/usr/include/readline)

add_executable(MedScheduler main.cpp users.cpp)

target_link_libraries(MedScheduler readline sqlite3)
