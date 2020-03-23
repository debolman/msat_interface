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
    fd = open("/dev/cu.usbmodem14601",O_RDWR );
    if(fd == -1) printf("\n  Error! in Opening ttyUSB0  ");
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
    SerialPortSettings.c_cc[VTIME] = 1;
    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) printf("\n  ERROR ! in Setting attributes");
}

void *serial_listen(void *vargp)
{
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
                //system("echo -e "\a"");
            }
            if(read_buffer[0] == 0x77) {
                tic = timee();
                printf("%llu \n", tic);
                
                toc = tic;
            }
            if(read_buffer[0] == 0x50) {
                memcpy(&tlm, read_buffer,bytes_read);
                //printf("%d \n", tlm.rssi);
                char a[30];
                sprintf(a,"%d",tlm.rssi);
                gtk_label_set_text(GTK_LABEL(rssi_v)  ,a);
                sprintf(a,"%d",tlm.snr);
                gtk_label_set_text(GTK_LABEL(snr_v)  ,a);
                sprintf(a,"%d",tlm.milis);
                gtk_label_set_text(GTK_LABEL(millis_v)  ,a);
                sprintf(a,"%d",tlm.freq_er_hz);
                gtk_label_set_text(GTK_LABEL(freq_err_v)  ,a);
            }
        }
        usleep(10000);
    }
    return NULL;
}
