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
#include <time.h>
#include <sys/timeb.h>  
#include <sys/time.h>

int fd, fo, bytes_read;
struct  tm *ts;
char    buff[80];
bool serial_raw = false;
const int pkt_size = 255;

typedef struct {
    int id;
    double latitud;
    int sat_n;
    double longitud;
    int unix_time;
    double altitud;
}  tlm_sct;

void serial_initialize() {
    fd = open("/dev/ttyS0",O_RDWR | O_NOCTTY );//| O_NDELAY);
    if(fd == -1)
        printf("\n  Error! in Opening ttyUSB0  ");
    struct termios SerialPortSettings;
    tcgetattr(fd, &SerialPortSettings);
    cfsetispeed(&SerialPortSettings,B57600);
    cfsetospeed(&SerialPortSettings,B57600);
    SerialPortSettings.c_cflag &= ~PARENB;
    SerialPortSettings.c_cflag &= ~CSTOPB;
    SerialPortSettings.c_cflag &= ~CSIZE;
    SerialPortSettings.c_cflag |=  CS8;
    SerialPortSettings.c_cflag &= ~CRTSCTS;
    SerialPortSettings.c_cflag |= CREAD | CLOCAL;
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);
    SerialPortSettings.c_iflag &= ~(  ECHO | ECHOE | ISIG);
    SerialPortSettings.c_oflag &= ~OPOST;
    SerialPortSettings.c_cc[VMIN] = pkt_size; /* Read at least x characters */
    SerialPortSettings.c_cc[VTIME] = 3; /* Wait 3 deciseconds   */
    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) printf("\n  ERROR ! in Setting attributes");
}

void *serial_listen(void *vargp)
{
    while(true) {
        tcflush(fd, TCIFLUSH);
        bytes_read = 0;
        char read_buffer[pkt_size];
        bytes_read = read(fd,&read_buffer,pkt_size);
        //printf("%d \n",bytes_read );
        
        tlm_sct* tlm = (tlm_sct*)read_buffer;
        ts = localtime( &tlm->unix_time );
        strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S   ", ts);
        
        //printf("%.9f %d %s \n", tlm->latitud, tlm->unix_time, buff);
        
        if(bytes_read >0) {
            if(serial_raw) {
                for(int n =0;n<bytes_read;n++)
                    printf("%d ",read_buffer[n]);
                printf("\n");
            }
            if(read_buffer[0] == 57) {
                char bu[pkt_size-1];
                memcpy(bu,read_buffer+1,sizeof(bu));
                int scritti=write(fo,bu,sizeof(bu));
            }
        }
    }
    return NULL;
}

int main(void)
{
    serial_initialize();
    pthread_t serial;

    
    fo = open("picture.jpg",O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0666 );//| O_NDELAY);
    if(fo == -1)
        printf("\n  Error! in Opening file  ");
    
    pthread_create(&serial, NULL, serial_listen, NULL);
    pthread_join(serial, NULL);
    
    fo = open("picture.jpg",O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0666 );//| O_NDELAY);
    if(fo == -1)
        printf("\n  Error! in Opening file  ");
    
    
    
    close(fd);
}

