#!/bin/bash
rm interface
gcc -pthread main.c -o interface
./interface
