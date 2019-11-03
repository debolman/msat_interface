#!/bin/bash
rm interface
#gcc -pthread main.c -o interface -I /usr/local/Cellar/mariadb-connector-c/3.1.2_1/include/mariadb  -I /usr/local/Cellar/mysql-client/5.7.23_1/include/mysql/
gcc -pthread main.c -o interface 
./interface
