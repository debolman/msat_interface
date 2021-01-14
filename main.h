//
//  main.c
//  interface
//
//  Created by Diego Amadio on 4/10/19.
//  Copyright © 2019 Diego Amadio. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdbool.h>
#include <netdb.h>
#include <stdbool.h>

bool  serial_activate =       true;
bool  UDP_activate   =        false;
bool  mysql_activate =        false;
bool  file_activate  =        false;
bool  TCP_server_activate  =    true;
bool  TCP_client_activate  =  false;
bool  TCP_server_beacon  =  false;
bool serial_raw = false;
bool udp_raw = false;
bool TCP_raw = false;

#define MYSQL_act_marco
//#define MAXEVENTS 64
#define pkt_size  128
#define serial_baudrate B230400
#define UDP_serv_port 1235
#define TCP_serv_port 8082
#define TCP_dest_port 8082

#ifndef MYSQL_act_marco
    #include <mysql.h>
    MYSQL *con;
#endif


char hostbuffer[] = "debolman.ns0.it";
char TCP_dest_addr[] = "10.8.0.1";
//char serial_port[] = "/dev/cu.SLAB_USBtoUART";
//char serial_port[] = "/dev/ttyUSB0";
//char serial_port[] = "/dev/cu.usbmodem14501";
//char serial_port[] = "/dev/cu.usbserial-FT2GNWLC";
char serial_port[] = "/dev/cu.usbserial-FT0HBRMT";

int serial_file_descriptor, bytes_read, TCP_client_socket, UDP_socket, len, master_socket, activity, sd, addrlen, new_socket, max_sd;
pthread_t serial_thread, udp_thread, mysql_thread, file_thread, timer_thread, tcp_serv_thread, tcp_cli_thread, tcp_rec, timer, tcp_serv_beacon_thread;
unsigned char UDP_buffer[1024];
unsigned char bufer[1025];
char command[10][32];
int client_socket[30];
int max_clients = 4;;
fd_set readfds;
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void decode_tlm();
unsigned long long unix_seconds();
unsigned long long unix_milliseconds();
void UDP_send(unsigned char *hello, int leng);
void red();
void green();
void yellow();
void blue();
void magenta();
void print_green();
void cyan();
void white() ;
void normal() ;
void write_wo_connection( int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);

struct parameters {
  int8_t id;
  int8_t SF;
  int8_t coding;
  int8_t crc;
  int8_t pwr_db;
  int8_t pwr_pa;
  int8_t band_i;
  int8_t freq_i;
  int8_t beacon;
    int32_t milis;
} param;


