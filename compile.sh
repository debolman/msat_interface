#!/bin/bash
rm interface
cc -pthread -o interface `mariadb_config --cflags --libs --include`  ` pkg-config --cflags --libs gtk+-3.0` main.c
#gcc -pthread -o interface `mariadb_config --cflags --libs --include` main.c
./interface
