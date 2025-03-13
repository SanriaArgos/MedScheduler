# Базовый образ
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Устанавливаем необходимые пакеты
RUN apt-get update && apt-get install -y --fix-missing \
    cmake \
    wget \
    git \
    libboost-all-dev \
    libpq-dev \
    libssl-dev \
    postgresql \
    postgresql-contrib \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем Boost.Beast через Boost (отдельно не нужен)

# Скачиваем json.hpp
RUN mkdir -p /usr/local/include/nlohmann && \
    wget -q https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp -O /usr/local/include/nlohmann/json.hpp

# Устанавливаем рабочую директорию
WORKDIR /app

# Копируем файлы проекта в контейнер
COPY . .

# Удаляем старые артефакты сборки, если они есть
RUN rm -rf build CMakeCache.txt CMakeFiles

# Создаем каталог сборки, переходим в него, запускаем cmake и сборку
RUN mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j$(nproc)

# Указываем порт
EXPOSE 8080

# Запускаем приложение
CMD ["./build/boost_api_server"]