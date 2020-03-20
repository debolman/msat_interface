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
//#include <mysql.h>
#include <netdb.h>

<<<<<<< HEAD
#define pkt_size  1
#define diff_size 1
#define MAXLINE 1024
#define serv_port    7071

#define serial_activate 1
#define udp_activate    0
#define ram_allocation  0
#define mysql_activate  0
#define file_activate  0

=======
#define MAXEVENTS 64
#define pkt_size  128
#define diff_size 1
#define MAXLINE 1024
#define udp_serv_port    6070
#define tcp_serv_port    6080

#define serial_activate 0
#define tcp_activation    10
#define udp_activate    1
#define ram_allocation  0
#define mysql_activate  0
>>>>>>> 9c21ece14ad9f2d7472f20d0ed62e65ca5292daf

int fd, fo, bytes_read, sockfd, len;
struct  tm *ts;
char udp_buffer[MAXLINE];
char command[10][32];
bool serial_raw = false;
<<<<<<< HEAD
bool udp_raw = true;
pthread_t serial_thread, udp_thread, udp_sample_thread, mysql_thread,log_thread, file_thread, timer_thread;
=======
bool udp_raw = false;
pthread_t serial_thread, udp_thread, udp_sample_thread, mysql_thread,log_thread, tcp_thread;
>>>>>>> 9c21ece14ad9f2d7472f20d0ed62e65ca5292daf
struct timeb timer_msec;
long long int timestamp_msec, t_o, t_n, t_d;
char udp_buffer[MAXLINE];
struct sockaddr_in servaddr, cliaddr, rx_addr;
char hostbuffer[] = "debolman.ns0.it";
//MYSQL *con;
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
<<<<<<< HEAD
void UDP_send(char *hello, int leng);
=======
void UDP_ssend(char*, int );
>>>>>>> 9c21ece14ad9f2d7472f20d0ed62e65ca5292daf
