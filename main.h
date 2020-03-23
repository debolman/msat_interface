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
#include <gtk/gtk.h>

GtkWidget  *box_itm, *window, *box_pay, *frame, *box_main, *box;
GtkWidget  *pwr_v, *snr_v, *button, *switcher, *rssi_v, *label, *switch_pa, *millis_v,  *pwr_sca, *ip_l, *ip_l2, *ip_l3, *freq_err_v,  *unix_time_l;
GtkWidget  *udp_serv_port_v, *udp_raw_switch, *udp_act_s;
GtkWidget  *tcp_serv_port_v, *tcp_raw_switch, *tcp_act_s;
GtkWidget  *serial_act_s,  *serial_raw_switch, *ser_pkt_size_v, *serial_act_s;

#define MAXEVENTS 64
#define pkt_size  36
#define diff_size 1
#define MAXLINE 1024
#define udp_serv_port    6070
#define tcp_serv_port    6080

#define serial_activate false
#define tcp_activation  false
#define udp_activate    false
#define ram_allocation  false
#define mysql_activate  false
#define file_activate   false
#define tcp_server      false
#define GUI_activation  false

int fd, fo, bytes_read, sockfd, len;
struct  tm *ts;
unsigned char udp_buffer[MAXLINE];
char command[10][32];
bool serial_raw = false;
bool udp_raw = false;
bool tcp_raw = false;
pthread_t serial_thread, udp_thread, udp_sample_thread, mysql_thread,log_thread, tcp_thread, file_thread, timer_thread, GUI_thread;
struct timeb timer_msec;
long long int timestamp_msec, t_o, t_n, t_d;
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

struct tlm_sct {
  char id;
  char temp2;
  char leng;
  char te;
  int16_t rssi; //4
  int16_t snr;
  int32_t freq;
  int32_t freq_er_hz;
  int32_t milis;
  int16_t SF;
  int8_t pwr_db;
  int8_t pwr_pa;
  int16_t coding_rate;
  int32_t band;
  bool act_sender;
} tlm;


void *log_thd();
unsigned long long  timee();
void UDP_send(unsigned char *hello, int leng);
