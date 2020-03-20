#!/bin/bash
rm interface
cc -pthread -o interface `mariadb_config --cflags --libs --include` main.c

gcc -pthread -o interface `mariadb_config --cflags --libs --include` main.c
./interface
