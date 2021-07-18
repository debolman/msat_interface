#!/bin/bash
#apt-get update  # To get the latest package lists
#apt-get install default-libmysqlclient-dev -y
rm interface
#cc -pthread -o interface `mariadb_config --cflags --libs --include`  main.c 
cc -pthread -o interface  main.c
./interface
