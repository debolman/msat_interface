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
#include "main.h"
#include "serial.c"
#include "mysql.c"
#include "net_UDP.c"

unsigned long long  timee() {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    unsigned long long millisecondsSinceEpoch =
        (unsigned long long)(tv.tv_sec) * 1000 +
        (unsigned long long)(tv.tv_usec) / 1000;

    //printf("%llu\n", millisecondsSinceEpoch);
    return millisecondsSinceEpoch;
}

void commands_strings() {
    strcpy(command[0], "udp");
    strcpy(command[1], "udp off");
    strcpy(command[2], "serial");
    strcpy(command[3], "serial off");
    strcpy(command[4], "close");
}





long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; 
    return milliseconds;
}



void timer() {
        toc = timee() - tic;
           printf("%llu \n",toc);
               tic = timee();
    
}


int main(void)
{
	commands_strings();
    if(serial_activate) serial_initialize();
    if (udp_activate) socket_initialize();
    if (udp_activate) pthread_create(&udp_thread, NULL, UDP_listener, NULL);
    if(serial_activate) pthread_create(&serial_thread, NULL, serial_listen, NULL);
    
    if (mysql_activate) pthread_create(&mysql_thread, NULL, mysql_log, NULL);
    pthread_join(mysql_thread, NULL);
    
    if(serial_activate) pthread_join(serial_thread, NULL);
    if (udp_activate) pthread_join(udp_thread, NULL);
    if(serial_activate) close(fd);
    if (udp_activate) close(sockfd);
}


