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
#include "main.h"
#include "serial.c"
#include "TCP.c"
#include "UDP.c"
//#include <mysql.h>
//#include "mysql.c"

unsigned long long  unix_milliseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long long millisecondsSinceEpoch =
        (unsigned long long)(tv.tv_sec) * 1000 +
        (unsigned long long)(tv.tv_usec) / 1000;
    return millisecondsSinceEpoch;
}

void time_human() {
  struct timeval  now;
  struct tm*      local;

  gettimeofday(&now, NULL);
  local = localtime(&now.tv_sec);
    char stri[] = {"%02d%02d%02d-%02d%02d%02d"};
  sprintf(time_string,stri,local->tm_year+1900, local->tm_mon+1, local->tm_mday,local->tm_hour, local->tm_min, local->tm_sec);
}

void red() {printf("%s", KRED);}
void green() {printf("%s", KGRN);}
void yellow() {printf("%s", KYEL);}
void blue() {printf("%s", KBLU);}
void magenta() {printf("%s", KMAG);}
void cyan() {printf("%s", KCYN);}
void white() {printf("%s", KWHT);}
void normal() {printf("%s", KNRM);}


void *file_management() {
    printf("open \n");
    int file  =open("pic.jpg", O_RDONLY );
    unsigned char buffer_file[128];
    for(int n =0;n<600000;n++) {
        int letti = read(file,buffer_file,128);
        UDP_send(buffer_file,128);
        usleep(20000);
        printf("%d %02X %02X %02X\n",n, buffer_file[0], buffer_file[1], buffer_file[2]);
    }
    printf("end \n");
    return 0;
}

int main(void) {
    green();
    printf("Hello!\n");
    normal();
    if (serial_activate) serial_initialize();
    if (UDP_activate) socket_initialize();
    if (UDP_activate) pthread_create(&udp_thread, NULL, UDP_listener, NULL);
    if (file_activate) pthread_create(&file_thread, NULL, file_management, NULL);
    if (serial_activate) pthread_create(&serial_thread, NULL, serial_listen, NULL);
    if (TCP_server_activate) pthread_create(&tcp_serv_thread, NULL, tcp_serv_conn, NULL);
    if (TCP_client_activate) pthread_create(&tcp_cli_thread, NULL, tcp_cli, NULL);
    //if (mysql_activate) pthread_create(&mysql_thread, NULL, mysql_log, NULL);
    //if (mysql_activate) mysql_connection();
    //if (mysql_activate) pthread_join(mysql_thread, NULL);
    if (TCP_client_activate) pthread_join(tcp_cli_thread, NULL);
    if (TCP_server_activate) pthread_join(tcp_serv_thread, NULL);;
    if (serial_activate) pthread_join(serial_thread, NULL);
    if (UDP_activate) pthread_join(udp_thread, NULL);
}

