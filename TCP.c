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

void *tcp_cient_count () {
    for(;;) {
        sleep(10);
        printf("Connected clients: ");
        for (int i = 0; i < max_clients; i++)
        {
            if( client_socket[i] != 0 ) printf(" %d" , i);
        }
        printf("\n");
    }
}

void *tcp_serv_beacon() {
    for(;;) {
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if (FD_ISSET( sd , &readfds))
            {
                unsigned long long a = (unsigned long long)time(NULL);
                memcpy(bufer,(char *)&a,4);
                send(sd , bufer , 4 , 0 );
            }
        }
	sleep(1);
    }
}

void *TCP_client_receive() {
    for(;;) {
        bzero(bufer,1024);
        int num = read(TCP_client_socket,bufer,255);
        //printf("%d ",num);
        if(TCP_raw) {
            for (int n=0;n<num;n++) {
                printf("%02X ",bufer[n]);
            }
            printf("\n");
        }
        unsigned long long a;
        memcpy(&a,bufer,4);
        printf("%llu \n",a);
    }
}

void *tcp_server_receiver() {
    for(;;) {
        bzero(bufer,1024);
        int num = read(master_socket,bufer,255);
        //printf("%d ",num);
        if(TCP_raw & (num >0)) {
            for (int n=0;n<num;n++) {
                printf("%02X ",bufer[n]);
            }
            printf("\n");
        }
    }
}


void *tcp_cli() {

        int connfd;
        struct sockaddr_in servaddr, cli;
    int optval;
    socklen_t optlen = sizeof(optval);
    
    for(;;) {
        TCP_client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (TCP_client_socket == -1) {
            red();
            printf("socket creation failed...\n");
            normal();
            exit(0);
        }
        else
        {
            green();
            printf("TCP Socket successfully created\n");
            normal();
        }
        bzero(&servaddr, sizeof(servaddr));
      
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(TCP_dest_addr);
        servaddr.sin_port = htons(TCP_dest_port);

        // connect the client socket to server socket
        if (connect(TCP_client_socket, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
            red();
            printf("connection with the server failed...\n");
            normal();
            exit(0);
        }
        else {
            green();
            printf("Connected to %s:%d\n", TCP_dest_addr, TCP_dest_port);
            normal();
        }
     
            //pthread_create(&timm, NULL, &tcp_cli_send, NULL);
        pthread_create(&tcp_rec, NULL, TCP_client_receive, NULL);
        pthread_join(tcp_rec, NULL);
    }
}

void *tcp_serv_conn() {
    struct sockaddr_in TCP_server_address;
    int opt = 1;
    //initialise all client_socket[] to 0 so not checked
    for (int i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }
    
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        red();
        perror("socket failed");
        normal();
        exit(EXIT_FAILURE);
    }
  
    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
  
    //type of socket created
    TCP_server_address.sin_family = AF_INET;
    TCP_server_address.sin_addr.s_addr = INADDR_ANY;
    TCP_server_address.sin_port = htons( TCP_serv_port );
      
    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&TCP_server_address, sizeof(TCP_server_address))<0)
    {
        red();
        perror("bind failed");
        normal();
        exit(EXIT_FAILURE);
    }
    green();
    printf("TCP listening on port %d \n", TCP_serv_port);
    normal();
     
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 2) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
      
    //accept the incoming connection
    addrlen = sizeof(TCP_server_address);
    green();
    puts("Waiting for TCP clients...");
    normal();
    
//    pthread_create(&timer, NULL, tcp_cient_count, NULL);
    if(TCP_server_beacon) pthread_create(&tcp_serv_beacon_thread, NULL, tcp_serv_beacon, NULL);
    //pthread_join(timer, NULL);
    //if(tcp_serv_beacon_activate) pthread_join(tcp_serv_beacon_thread, NULL);
     
    for(;;)
    {
        //clear the socket set
        FD_ZERO(&readfds);
  
        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
         
        //add child sockets to set
        for (int  i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];
             
            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);
             
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }
  
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }
          
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&TCP_server_address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            //inform user of socket number - used in send and receive commands
            printf("New connection on socket %d from: %s:%d\n" , new_socket , inet_ntoa(TCP_server_address.sin_addr) , ntohs(TCP_server_address.sin_port));

            //add new socket to array of sockets
            for (int i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
//                    printf("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
        }
          
        //else its some IO operation on some other socket :)
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the incoming message
                if ((bytes_read = read( sd , bufer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&TCP_server_address , (socklen_t*)&addrlen);
                    printf("Host disconnected: %s:%d \n" , inet_ntoa(TCP_server_address.sin_addr) , ntohs(TCP_server_address.sin_port));
                      
                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }
				if(bytes_read>0) {
					if(serial_activate) {   int wrote_bytes =  write(serial_file_descriptor,&bufer,bytes_read);
					printf("TCP: %d %02X %02X %02X\n", wrote_bytes, bufer[0], bufer[1], bufer[2]);
					}
					if(TCP_raw) {
						for (int n=0;n<bytes_read;n++) {
							printf("%02X ",bufer[n]);
						}
						printf("\n");
					}
				}
            }
        }
    }
    return 0;
}
