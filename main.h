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
#include <mysql.h>
#include <netdb.h>

#define pkt_size  255
#define diff_size 1
#define MAXLINE 1024
#define serv_port    6070

#define serial_activate 1
#define udp_activate    0
#define ram_allocation  0
#define mysql_activate  0

int fd, fo, bytes_read, sockfd, len;
struct  tm *ts;
char udp_buffer[MAXLINE];
char command[10][32];
bool serial_raw = false;
bool udp_raw = false;
pthread_t serial_thread, udp_thread, udp_sample_thread, mysql_thread,log_thread;
struct timeb timer_msec;
long long int timestamp_msec, t_o, t_n, t_d;
char udp_buffer[MAXLINE];
struct sockaddr_in servaddr, cliaddr, rx_addr;
char hostbuffer[] = "debolman.ns0.it";
MYSQL *con;
unsigned long long toc, tic;
int counter = 0;

typedef struct {
    int id;
    double latitud;
    int sat_n;
    double longitud;
    int unix_time;
    double altitud;
}  tlm_sct;

//void log_thd();
unsigned long long  timee();
