cmake_minimum_required(VERSION 3.16)
project(MedSchedulerClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE CLIENT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

add_executable(med_client ${CLIENT_SOURCES})

target_link_libraries(med_client
    PRIVATE
        nlohmann_json::nlohmann_json
        CURL::libcurl
)
target_include_directories(med_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
