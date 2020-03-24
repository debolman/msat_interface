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
#include <mysql.h>

const bool  serial_activate = false;
const bool  udp_activate   =  true;
const bool  ram_allocation  = false;
const bool  mysql_activate =  false;
const bool  file_activate  =  false;

#define MAXEVENTS 64
#define pkt_size  44
#define diff_size 1
#define MAXLINE 1024
#define udp_serv_port    6070
#define tcp_serv_port    6080
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


MYSQL *con;
int fd, fo, bytes_read, sockfd, socket_fd, len;
struct  tm *ts;
unsigned char udp_buffer[MAXLINE];
char command[10][32];
bool serial_raw = false;
bool udp_raw = true;
pthread_t serial_thread, udp_thread, udp_sample_thread, mysql_thread,log_thread, tcp_thread, file_thread, timer_thread, GUI_thread;
struct timeb timer_msec;
long long int timestamp_msec, t_o, t_n, t_d;
struct sockaddr_in servaddr, cliaddr, rx_addr;
char hostbuffer[] = "debolman.ns0.it";
unsigned long long toc, tic;
int counter = 0;
int sent_counter = 0;
int recv_counter = 0;

typedef struct {
    int id;
    double latitud;
    int sat_n;
    double longitud;
    int unix_time;
    double altitud;
}  tlm_sct;

struct tlm_sct {
  char id;
  char leng;
  int16_t freq_er_hz;
  int16_t rssi; //4
  int8_t snr;
  bool act_sender;
  int32_t milis_r;
  int32_t milis_p;
  int8_t SF;
  int8_t pwr_db;
  int8_t pwr_pa;
  int8_t coding_rate;
  int32_t band;
} tlm;

void decode_tlm();
void *log_thd();
unsigned long long  timee();
void UDP_send(unsigned char *hello, int leng);
void red();
void green();
void yellow();
void blue();
void magenta();
void cyan();
void white() ;
void normal() ;
