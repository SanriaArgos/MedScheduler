## MedScheduler
**Сервис по поиску врачей и записи на прием.**

**Авторы**: ```Бальжиев Аюр```, ```Грудцына Виктория```, ```Толстограева Виктория```

Клиент-серверное приложение для управления медицинскими записями. Сервер и база данных работают в изолированном контейнере, а клиент представляет собой локальное Qt-приложение с графическим интерфейсом. Взаимодействие между клиентом и сервером осуществляется по HTTP с использованием JSON.

## Используемые технологии

| Библиотека            | Назначение                                                       |
|------------------------|------------------------------------------------------------------|
| **Qt 5**               | Графический интерфейс (клиентская часть)                         |
| **Boost.Beast**        | Асинхронный HTTP-сервер                                          |
| **nlohmann/json**      | Формирование JSON-запросов и парсинг ответов                     |
| **libcurl**            | HTTP-клиент для взаимодействия клиента с сервером                |
| **libpq (PostgreSQL)** | Работа с базой данных на стороне сервера                         |
| **OpenSSL (Crypto)**   | SHA-256 хеширование паролей и генерация соли                     |



## Архитектура приложения

- Клиентская часть — десктопное Qt-приложение, использующее `libcurl` для отправки HTTP-запросов к серверу.
- Сервер реализован на C++ с использованием Boost.Beast и обрабатывает HTTP-запросы асинхронно.
- Все данные передаются в формате JSON, сторонние заголовки не используются.
- Сервер взаимодействует с базой данных PostgreSQL через `libpq`.
- Хеширование паролей выполняется с использованием SHA-256 из OpenSSL.
- Взаимодействие осуществляется через `localhost`, авторизация пока не реализована.
    
## Исходный код
На данном этапе код MVP-проекта реализован в ветке [QtApp](https://github.com/SanriaArgos/MedScheduler/tree/QtApp).
##  Запуск сервера и базы данных
После запуска Docker выполнить следующую команду:
```bash
docker-compose up --build
```

## Сборка и запуск клиента
 ```bash
qmake MedScheduler.pro
make
./MedScheduler
```

 ## Команда для форматирования
  ```bash
 find . -type f \( -iname '*.cpp' -o -iname '*.hpp' -o -iname '*.h' -o -iname '*.c' \) -print -exec clang-format-15 -i {} +
```




index index.html index.htm index.nginx-debian.html;
server_name api.medscheduler.ru www.medscheduler.ru medscheduler.ru; # managed by Certbot


        location / {
                # First attempt to serve request as file, then
                # as directory, then fall back to displaying a 404.
                try_files $uri $uri/ =404;
        }

        # pass PHP scripts to FastCGI server
        #
        #location ~ \.php$ {
        #       include snippets/fastcgi-php.conf;
        #
        #       # With php-fpm (or other unix sockets):
        #       fastcgi_pass unix:/run/php/php7.4-fpm.sock;
        #       # With php-cgi (or other tcp sockets):
        #       fastcgi_pass 127.0.0.1:9000;
        #}

        # deny access to .htaccess files, if Apache's document root
        # concurs with nginx's one
        #
        #location ~ /\.ht {
        #       deny all;
        #}


    listen [::]:443 ssl ipv6only=on; # managed by Certbot
    listen 443 ssl; # managed by Certbot
    ssl_certificate /etc/letsencrypt/live/medscheduler.ru/fullchain.pem; # managed by Certbot
    ssl_certificate_key /etc/letsencrypt/live/medscheduler.ru/privkey.pem; # managed by Certbot
    include /etc/letsencrypt/options-ssl-nginx.conf; # managed by Certbot
    ssl_dhparam /etc/letsencrypt/ssl-dhparams.pem; # managed by Certbot


