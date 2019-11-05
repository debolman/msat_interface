#!/bin/bash
rm interface
#gcc -pthread -o interface `mariadb_config --cflags --libs --include` main.c
gcc -pthread -o interface -I/usr/local/Cellar/mariadb-connector-c/3.1.2_1/include/mariadb `mariadb_config --cflags --libs` main.c
./interface
