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

#define pkt_size  40
#define diff_size 1
#define MAXLINE 1024
#define serv_port    6070

int fd, fo, bytes_read, sockfd, len;
struct  tm *ts;
char udp_buffer[MAXLINE];
char command[10][10];
bool serial_raw = false;
pthread_t serial, udp_thread;
struct timeb timer_msec;
long long int timestamp_msec, t_o, t_n, t_d;
char udp_buffer[MAXLINE];
struct sockaddr_in servaddr, cliaddr, rx_addr;

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
    cfsetispeed(&SerialPortSettings,B9600);
    cfsetospeed(&SerialPortSettings,B9600);
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

void commands_strings() {
    strcpy(command[0], "udp");
    strcpy(command[1], "udp off");
    strcpy(command[2], "serial");
    strcpy(command[3], "serial off");
    strcpy(command[4], "sat");
    strcpy(command[5], "sat off");
    strcpy(command[6], "e");
    strcpy(command[7], "serial raw");
    strcpy(command[8], "serial raw off");
    strcpy(command[9], "udp raw");
    strcpy(command[10], "udp raw off");
    strcpy(command[11], "open");
    strcpy(command[12], "next");
    strcpy(command[13], "download");
    strcpy(command[14], "delete");
    strcpy(command[15], "close");
    strcpy(command[16], "restart");
}


void *serial_listen(void *vargp)
{
    while(true) {
        tcflush(fd, TCIFLUSH);
        bytes_read = 0;
        char read_buffer[pkt_size];
        bytes_read = read(fd,&read_buffer,pkt_size);
        printf("%d %d %d %d \n",  bytes_read, read_buffer[0], read_buffer[1], read_buffer[2]);
        
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
                sprintf(str, "%lu.png", (unsigned long)time(NULL));
                fo = open(str,O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0666 );//| O_NDELAY);
                if(fo == -1)
                    printf("\n  Error! in Opening file  ");
            }
        }
    }
    return NULL;
}
void *UDP_listener(void *vargp)
{
    while(true) {
        len = sizeof(cliaddr); 
        int UDP_recved_len = recvfrom(sockfd, udp_buffer, sizeof(udp_buffer), 0, (struct sockaddr*)&cliaddr,&len); 
        if(UDP_recved_len>0) {
                for(int n =0 ; n<UDP_recved_len;n++) printf("%d ", udp_buffer[n]);
                printf("\n");
        }
	int wrote_bytes =  write(fd,&udp_buffer,UDP_recved_len);
    }
}
long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; 
    return milliseconds;
}


void socket_initialize() {
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(serv_port);
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void UDP_ssend(char *hello, int leng) {
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(6060);
    cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    sendto(sockfd, (const char *)hello, leng, 0, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
}
int main(void)
{
    serial_initialize();
    socket_initialize();
	long long ti = current_timestamp();
    pthread_create(&udp_thread, NULL, UDP_listener, NULL);
    pthread_create(&serial, NULL, serial_listen, NULL);         
    pthread_join(serial, NULL);
    pthread_join(udp_thread, NULL);
    
    char cmd[255];
    memset(cmd, 0, 255);
    do
    {
        fgets(cmd, 255, stdin);
        cmd[strcspn ( cmd, "\n")] = '\0';
        if(!strcmp(cmd, command[0])) ;
        else if(!strcmp(cmd, command[1])) ;
        
    } while(strcmp(cmd, "exit"));
    close(fd);
    close(sockfd);
}


