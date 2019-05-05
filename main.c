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

#define pkt_size  255
#define diff_size 1
#define MAXLINE 1024
#define sat_port     7070
#define serv_port    7071

int fd, fo, bytes_read, sockfd;
struct  tm *ts;
char    buff[80];
bool serial_raw = false;
pthread_t serial, udp_thread;
struct timeb timer_msec;
long long int timestamp_msec, t_o, t_n, t_d;
unsigned char udp_buffer[MAXLINE];
struct sockaddr_in servaddr, cliaddr;

typedef struct {
    int id;
    double latitud;
    int sat_n;
    double longitud;
    int unix_time;
    double altitud;
}  tlm_sct;

void serial_initialize() {
    fd = open("/dev/ttyS2",O_RDWR );
    if(fd == -1)
        printf("\n  Error! in Opening ttyUSB0  ");
    struct termios SerialPortSettings;
    tcgetattr(fd, &SerialPortSettings);
    cfsetispeed(&SerialPortSettings,B115200);
    cfsetospeed(&SerialPortSettings,B115200);
    SerialPortSettings.c_cflag &= ~PARENB;
    SerialPortSettings.c_cflag &= ~CSTOPB;
    SerialPortSettings.c_cflag &= ~CSIZE;
    SerialPortSettings.c_cflag |=  CS8;
    SerialPortSettings.c_cflag &= ~CRTSCTS;
    SerialPortSettings.c_cflag |= CREAD | CLOCAL;
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);
    SerialPortSettings.c_iflag &= ~(  ECHO | ECHOE | ISIG);
    SerialPortSettings.c_oflag &= ~OPOST;
    SerialPortSettings.c_cc[VMIN] = pkt_size;//pkt_size;
    SerialPortSettings.c_cc[VTIME] = 0.5; /* Wait 3 deciseconds   */
    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) printf("\n  ERROR ! in Setting attributes");
}

void *serial_listen(void *vargp)
{
    while(true) {
        unsigned long now = (unsigned long)time(NULL);
        if (!ftime(&timer_msec)) {
            timestamp_msec = ((long long int) timer_msec.time) * 1000ll + (long long int) timer_msec.millitm;
        }
        else {
            timestamp_msec = -1;
        }
        
        
        tcflush(fd, TCIFLUSH);
        bytes_read = 0;
        char read_buffer[pkt_size];
        bytes_read = read(fd,&read_buffer,pkt_size);
        
        /*tlm_sct* tlm = (tlm_sct*)read_buffer;
         ts = localtime( &tlm->unix_time );
         strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S   ", ts);*/
        
        //printf("%d     %d %.9f %.9f %d %s \n", now, tlm->sat_n, tlm->latitud, tlm->longitud, tlm->unix_time, buff);
        
        
        t_n =timestamp_msec;
        t_d = t_n- t_o;
        printf("%lld ",t_d);
        t_o = t_n;
        
        printf("%d %d %d %d %d \n", now, bytes_read, read_buffer[0], read_buffer[1], read_buffer[2]);
        
        UDP_ssend(&read_buffer,bytes_read);
        
        if(bytes_read >0) {
            if(serial_raw) {
                for(int n =0;n< bytes_read;n++)
                    printf("%d ",read_buffer[n]);
                printf("\n");
            }
            if(read_buffer[0] == 57) {
                char bu[pkt_size-diff_size];
                memcpy(bu,read_buffer+diff_size,sizeof(bu));
                int scritti=write(fo,bu,sizeof(bu));
            }
            if(read_buffer[0] == 56 || read_buffer[0] == 1 || read_buffer[0] == 1) {
                char str[11];
                sprintf(str, "%d.png", now);
                fo = open(str,O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0666 );//| O_NDELAY);
                if(fo == -1)
                    printf("\n  Error! in Opening file  ");
            }
        }
    }
    return NULL;
}

int main(void)
{
    serial_initialize();
    socket_initialize();
    UDP_ssend();
    //pthread_create(&udp_thread, NULL, UDP_listener, NULL);
    pthread_create(&serial, NULL, serial_listen, NULL);         //creation of the thread that contains the serial communication
    pthread_join(serial, NULL);
    
    close(fd);
}

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

void *UDP_listener(void *vargp)
{
    while(true) {
        int recved;
        socklen_t len;
        recved = recvfrom(sockfd, (char *)udp_buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        printf("%d \n",recved);
        write(fo,udp_buffer,recved);
        if(recved>0) {
            if(true) {
                for(int n =0 ; n<recved;n++) printf("%d ", udp_buffer[n]);
                printf("\n");
            }
            //int scritti=write(fo,udp_buffer,recved);
            if(udp_buffer[0] == 101) {
                char sat_buff[200];
                //memcpy(sat_buff,udp_buffer+1,recved-1);
                //RX(sat_buff,recved-1);
            }
        }
    }
}
void socket_initialize() {
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(serv_port);
    
    cliaddr.sin_family = AF_INET; // IPv4
    cliaddr.sin_addr.s_addr = inet_addr("192.168.3.91");;
    cliaddr.sin_port = htons(serv_port);
    
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        //exit(EXIT_FAILURE);
    }
}

void UDP_ssend(char *hello, int leng) {
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6060);
    servaddr.sin_addr.s_addr = inet_addr("192.168.3.54");
    
    sendto(sockfd, (const char *)hello, leng,
           0, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));
}
