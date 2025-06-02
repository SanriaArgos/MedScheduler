cmake_minimum_required(VERSION 3.10)
project(boost_api_server)

set(CMAKE_CXX_STANDARD 17)

# Поиск зависимостей
find_package(Boost REQUIRED COMPONENTS system)

find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Указываем пути к заголовочным файлам
include_directories(${Boost_INCLUDE_DIRS} ${PostgreSQL_INCLUDE_DIRS} include)

# Собираем все исходные файлы из директории src
file(GLOB SOURCES src/*.cpp src/handlers/*.cpp)

# Создаем исполняемый файл
add_executable(boost_api_server ${SOURCES})

# Связываем с библиотеками
target_link_libraries(boost_api_server 
    PRIVATE
    ${Boost_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
)