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

void serial_initialize() {

    //fd = open("/dev/cu.usbmodem14601",O_RDWR );
	fd = open("/dev/ttyS2",O_RDWR );
    if(fd == -1) {
        red();
        printf("Serial port error\n");
        normal();
    }
    else {
        green();
        printf("Serial port opened\n");
        normal();
    }
    struct termios SerialPortSettings;
    tcgetattr(fd, &SerialPortSettings);
    cfsetispeed(&SerialPortSettings,B19200);
    cfsetospeed(&SerialPortSettings,B19200);
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
    SerialPortSettings.c_cc[VTIME] = 1;
    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) {
        red();
        printf("ERROR ! in Setting attributes\n");
        normal();
    }
}

void *serial_listen(void *vargp)
{
    green();
    printf("Serial listening\n");
    normal();
    while(true) {
        tcflush(fd, TCIFLUSH);
        unsigned char read_buffer[pkt_size];
        bytes_read = read(fd,&read_buffer,pkt_size);
        
        //printf("%d %d %d %d \n",  bytes_read, read_buffer[0], read_buffer[1], read_buffer[2]);
        //if(udp_activate) UDP_send(read_buffer,bytes_read);
        if(bytes_read >0) {
            if(serial_raw) {
                for(int n =0;n< bytes_read;n++)
                    printf("%02X ",read_buffer[n]);
                printf("\n");
            }
            if(read_buffer[0] == 0x50) {
                printf("chjk");
                memcpy(&tlm, read_buffer,bytes_read);
            }
			for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if (FD_ISSET( sd , &readfds))
            {
                unsigned long long  a = unix_secs();
                memcpy(bufer,(char *)&a,4);
                send(sd , read_buffer , bytes_read , 0 );
            }
        }
        }
        usleep(10000);
    }
    return NULL;
}
