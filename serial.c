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

int serial_initialize() {
       serial_port = open(serial_port_name, O_RDWR);
  struct termios tty;
  if(tcgetattr(serial_port, &tty) != 0) {
      red();
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      normal();
      return 1;
  }
  else {
      green();
      printf("UART %s opened.. ", serial_port_name);
      normal();
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 1;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = pkt_size;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, serial_baudrate);
  cfsetospeed(&tty, serial_baudrate);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
  }
  return 0;
}

void *serial_listen(void *vargp)
{
    green();
    printf("Serial listening\n");
    normal();
        FILE *fp = fopen ("/Users/diego/pic.jpg","w");
    while(true) {
        uint8_t read_buffer[pkt_size];
        bytes_read = read(serial_port,&read_buffer,pkt_size);
        time_human();
        char preview[128] = {"%s - UART: %d - %02X %02X %02X %02X \n"};
        sprintf(print_buffer,preview, time_string, bytes_read, read_buffer[0], read_buffer[1], read_buffer[2], read_buffer[3]);
        printf("%s",print_buffer);
        if(bytes_read >0) {
            if(serial_raw) {
                for(int n =0;n< bytes_read;n++)
                    printf("%02X ",read_buffer[n]);
                printf("\n");
            }
		if(UDP_activate) UDP_send(read_buffer,bytes_read);
		if(TCP_server_activate) {
			for (int i = 0; i < max_clients; i++) {
		            	sd = client_socket[i];
        		   	if (FD_ISSET( sd , &readfds)) {
           	     		int se = send(sd , read_buffer , bytes_read , 0 );
                             
            		}
		        }
            }
        }
        if(bytes_read == 100 && read_buffer[0] == 0x40) {
            uint16_t num;
            memcpy(&num,read_buffer+2,2);
            picture_index = (num)*96;
                        printf("%d ",picture_index);
            memcpy(&picture_array[picture_index],&read_buffer[4],96);
            if (counter++>10) {
                save_picture_file(picture_array);
                counter=0;
            }
        }
        if(bytes_read == 100 && read_buffer[0] == 0x41) {
                    char filename[64] = {};
                    char buf[] = {"/Users/diego/pictures/%s.jpg"};
                    char buf2[] = {"/Users/diego/pic.jpg"};
                    sprintf(filename, buf,time_string);
                    //picture_file = fopen (filename,"w");
                    picture_file = fopen (buf2,"w");
                    fwrite(picture_array,1, picture_index, picture_file);
                    fclose(picture_file);
                    printf("file saved \n");
                    memset(picture_array, 0, sizeof(picture_array));
        }

        //         if(read_buffer[0] == 0x11) {
        //     printf("dect \n");
        //     if(read_buffer[1] == 1) {
        //         fclose(fp);
        //         printf("closed \n");
        //         usleep(10000);
        //         printf("opened\n");
        //         fp = fopen ("/Users/diego/pic.jpg","w");
        //     }
        // }
        // if(read_buffer[0] == 0x30) {
        //     memcpy(&bufer, &read_buffer[4],bytes_read-4);
        //     fwrite(bufer,1, bytes_read-4, fp);
        // }

        usleep(10000);
    }
    return NULL;
}

void save_picture_file(uint8_t picture_array[]) {
                        char filename[64] = {};
                    char buf[] = {"/Users/diego/pictures/%s.jpg"};
                    char buf2[] = {"/Users/diego/pic.jpg"};
                    sprintf(filename, buf,time_string);
                    //picture_file = fopen (filename,"w");
                    picture_file = fopen (buf2,"w");
                    fwrite(picture_array,1, picture_index, picture_file);
                    fclose(picture_file);
}
