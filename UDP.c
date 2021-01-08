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

void *UDP_listener(void *vargp)
{
    struct sockaddr_in rem_addr;
    green();
    printf(" and listening on port %d\n", UDP_serv_port);
    normal();
    while(true) {
        socklen_t len;
        int UDP_recved_len = recvfrom(UDP_socket, UDP_buffer, sizeof(UDP_buffer), 0, (struct sockaddr*)&rem_addr,&len);
        if(UDP_recved_len>0) {
            printf("UDP: %d %02X %02X %02X \n",  UDP_recved_len, UDP_buffer[0], UDP_buffer[1], UDP_buffer[2]);
        
            //UDP_send_f(udp_buffer,UDP_recved_len);
            
            if(udp_raw) {
              for(int n =0 ; n<UDP_recved_len;n++) printf("%02X ", UDP_buffer[n]);
                printf("\n");
            }
            if(UDP_buffer[0] == 0x70) {
                memcpy(&param, UDP_buffer,UDP_recved_len);
                #ifndef MYSQL_act_marco
                    write_wo_connection(param.id,param.SF, param.coding, param.crc, param.pwr_db, param.pwr_pa,  param.band_i, param.freq_i, param.beacon , param.milis);
                #endif
            }
            		if(TCP_server_activate) {
			for (int i = 0; i < max_clients; i++) {
		            	sd = client_socket[i];
        		   	if (FD_ISSET( sd , &readfds)) {
           	     		int se = send(sd , UDP_buffer , UDP_recved_len , 0 );
            		}
		
            }
        }
        }
        
    }
}


void socket_initialize() {
    struct sockaddr_in UDP_server_address;
    if ( (UDP_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        red();
        printf("UDP socket creation failed");
        normal();
        exit(EXIT_FAILURE);
    }
    else {
        green();
        printf("UDP socket activated..");
        normal();
    }
    bzero(&UDP_server_address, sizeof(UDP_server_address));
    UDP_server_address.sin_family = AF_INET; // IPv4
    UDP_server_address.sin_addr.s_addr = INADDR_ANY;
    UDP_server_address.sin_port = htons(UDP_serv_port);
    if ( bind(UDP_socket, (const struct sockaddr *)&UDP_server_address, sizeof(UDP_server_address)) < 0 )
    {
        red();
        printf("bind failed\n");
        normal();
        exit(EXIT_FAILURE);
    }
}

void UDP_send(unsigned char *hello, int leng) {
    struct sockaddr_in dest_addr;
    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(7072);
    dest_addr.sin_addr.s_addr = inet_addr("192.168.3.56");
    sendto(UDP_socket, (const char *)hello, leng, 0, (const struct sockaddr *) &dest_addr, sizeof(dest_addr));
}


