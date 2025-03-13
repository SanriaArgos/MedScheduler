cmake_minimum_required(VERSION 3.10)
project(boost_api_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)
find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${PostgreSQL_INCLUDE_DIRS} include)

file(GLOB SOURCES src/*.cpp)

add_executable(boost_api_server ${SOURCES})
target_link_libraries(boost_api_server 
    PRIVATE
    ${Boost_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
    OpenSSL::Crypto
)
