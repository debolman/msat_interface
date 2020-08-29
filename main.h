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
const bool  udp_activate   =  false;
const bool  ram_allocation  = false;
const bool  mysql_activate =  false;
const bool  file_activate  =  false;
const bool  tcp_serv_activate  =  false;
const bool  tcp_client_activate  =  true;

#define MAXEVENTS 64
#define pkt_size  44
#define diff_size 1
#define MAXLINE 1024
#define udp_serv_port    7072
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
bool udp_raw = false;
pthread_t serial_thread, udp_thread, udp_sample_thread, mysql_thread,log_thread, tcp_thread, file_thread, timer_thread, tcp_serv_thread, tcp_cli_thread, timm, tcp_rec;
struct timeb timer_msec;
long long int timestamp_msec, t_o, t_n, t_d;
struct sockaddr_in servaddr, cliaddr, rx_addr;
char hostbuffer[] = "debolman.ns0.it";
unsigned long long toc, tic;
int counter = 0;
int sent_counter = 0;
int recv_counter = 0;
#define TRUE   1
#define FALSE  0
#define PORT 8082
int opt = TRUE;
  int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 4 , activity, i , valread , sd;
  int max_sd;
  struct sockaddr_in address;
    
  char buffer[1025];  //data buffer of 1K
    
  //set of socket descriptors
  fd_set readfds;
    
  //a message
  char *message = "ECHO Daemon v1";

 pthread_t timer;


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
void print_green();
void cyan();
void white() ;
void normal() ;
void UDP_send_f(unsigned char *hello, int leng) ;
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


struct parameter {
  int8_t id;
    int8_t id1;
    int8_t id2;
    int8_t id3;
  float V5_v;   //
  float V5_i;
  float V5_p;   //
  float V12_v;
  float V12_i;  //
  uint64_t milis;
    float V12_p;
} param_green;
