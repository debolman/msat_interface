#!/bin/bash
rm interface
#gcc -pthread main.c -o interface -I /usr/local/Cellar/mariadb-connector-c/3.1.2_1/include/mariadb  -I /usr/local/Cellar/mysql-client/5.7.23_1/include/mysql/
gcc -pthread -o interface  -I/usr/include/mariadb `mariadb_config --cflags --libs` main.c 
#gcc -pthread main.c -o interface `mysql_config --cflags --libs`
./interface
