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
#ifdef GUI_activation
#include "gtk.c"
#endif
unsigned long long  timee() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long long millisecondsSinceEpoch =
        (unsigned long long)(tv.tv_sec) * 1000 +
        (unsigned long long)(tv.tv_usec) / 1000;
    return millisecondsSinceEpoch;
}

void commands_strings() {
    strcpy(command[0], "udp");
    strcpy(command[1], "udp off");
    strcpy(command[2], "serial");
    strcpy(command[3], "serial off");
    strcpy(command[4], "close");
}


void *timer() {
    while(1) {
//        //unsigned long long diff;
//        //diff = timee()-toc;
//        //printf("%lu %lu %lu\n",tic, toc, diff);
//        if(diff>4000) {
//        printf("\rlost: %llu",diff/3450);
//        }
//        fflush(stdout);
        usleep(1000000);
        struct timeval tv;
        gettimeofday(&tv, NULL);
             char a[30];
        sprintf(a,"%llu",(unsigned long long)(tv.tv_sec));
        gtk_label_set_text(GTK_LABEL(unix_time_l)  ,a);
    }
}

void *file_management() {
    printf("open \n");
    int file  =open("pic.jpg", O_RDONLY );
    unsigned char buffer_file[pkt_size];
    buffer_file[0] = 57;
    printf("read \n");
    for(int n =0;n<40000;n++) {
        
        int letti = read(file,buffer_file+1,pkt_size-1);
        UDP_send(buffer_file,pkt_size);
        usleep(10000);
        //printf("%d \n",n);
        printf("%d %d %02X %02X %02X\n",n, buffer_file[0], buffer_file[1], buffer_file[2], buffer_file[3]);
        
    }
    printf("end \n");
    return 0;
}

int main(void)
{
    toc = 0;
	commands_strings();
    pthread_create(&timer_thread, NULL, timer, NULL);
    if(serial_activate) serial_initialize();
    if (udp_activate) socket_initialize();
    //if (GUI_activation) pthread_create(&GUI_thread, NULL, GUI_act, NULL);
    if (udp_activate) pthread_create(&udp_thread, NULL, UDP_listener, NULL);
    if(file_activate) pthread_create(&file_thread, NULL, file_management, NULL);
    if(serial_activate) pthread_create(&serial_thread, NULL, serial_listen, NULL);
    if (mysql_activate) pthread_create(&mysql_thread, NULL, mysql_log, NULL);

    if(tcp_activation) pthread_create(&tcp_thread, NULL, tcp_server, NULL);

    #ifdef GUI_activation
    GUI_act();
#endif
    pthread_join(mysql_thread, NULL);
    if(serial_activate) pthread_join(serial_thread, NULL);
    if (udp_activate) pthread_join(udp_thread, NULL);
    if(serial_activate) close(fd);
    if (udp_activate) close(sockfd);
}


