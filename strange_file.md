<!-- Как запускать:

```bash
docker build -t boost_api_server .
docker run -p 3000:8080 -v $(pwd)/data:/app/data boost_api_server
```
где boost_api_server - что угодно любое  название , 3000 - порт, по которому в браузере обращаться, например http://localhost:3000, pwd подставит само путь по типу "C:\Users\vital\OneDrive\Документы\GitHub\http-server-cpp"

в папке data в проекте (который и есть "$(pwd)/data" или "C:\Users\vital\OneDrive\Документы\GitHub\http-server-cpp\data") лежать будет база данных, эта папка не будет пересоздаваться при новом 
```bash
docker build -t boost_api_server .
```

```bash
container_id = $(docker run -p 5432:5432 -e A=B ... --rm -d postgres:16)
docker logs $(container_id)
run_db.sh
$sqlconnect localhost:5432
``` -->