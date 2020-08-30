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

void *tcp_rx () {
    
    for(;;) {
        sleep(1000);
        printf("list ");
        for (i = 0; i < max_clients; i++)
        {
            if( client_socket[i] != 0 )
            {
                printf(" %d" , i);
            }
        }
        printf("\n");

    }
}

void *tcp_serv_beacon() {
    while(true) {
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if (FD_ISSET( sd , &readfds))
            {
                    send(sd , "abc" , 3 , 0 );
            }
        }
    }
}

void* tcp_cli_send() {
    while(1) {
        printf("%llu \n", unix_secs() );
        unsigned long ti = unix_secs();
        memcpy(buff,(unsigned char *)&ti,4);
    int g = write(sockfd, buff, 4);
    //printf("write: %d\n",g);
        sleep(1000);
    }
}

void *tcp_cli_recv() {
    while(1) {
        bzero(buffer,256);
        int n = read(sockfd,buffer,255);
        printf("%d %s\n",n, buffer);
    }
}

void *tcp_cli() {

        int sockfd, connfd;
        struct sockaddr_in servaddr, cli;
    
    int optval;
    socklen_t optlen = sizeof(optval);

    
    while(1) {
          printf("while.. \n");
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            printf("socket creation failed...\n");
            exit(0);
        }
        else
            printf("Socket successfully created..\n");
        bzero(&servaddr, sizeof(servaddr));
      

        /* Set the option active */
//         optval = 1;
//         optlen = sizeof(optval);
//         if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
//            perror("setsockopt()");
//            close(sockfd);
//            exit(EXIT_FAILURE);
//         }
//         printf("SO_KEEPALIVE set on socket\n");

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("10.8.0.1");
        servaddr.sin_port = htons(TCP_dest_port);

        // connect the client socket to server socket
        if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
            printf("connection with the server failed...\n");
            exit(0);
        }
        else
            printf("connected to the server..\n");
     
            //pthread_create(&timm, NULL, &tcp_cli_send, NULL);
        pthread_create(&tcp_rec, NULL, tcp_cli_recv, NULL);
            //pthread_join(timm, NULL);
        pthread_join(tcp_rec, NULL);
        
        printf("join exit\n");
          
            // close the socket
            //close(sockfd);

        printf("end while.. \n");
    }
    
}

void *tcp_serv_conn()
{
    int opt = 1;
    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }
      
    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
  
    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( TCP_serv_port );
      
    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", TCP_serv_port);
     
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 2) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
      
    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
    
    
    pthread_create(&timer, NULL, tcp_rx, NULL);
    if(tcp_serv_beacon_activate) pthread_create(&tcp_serv_beacon_thread, NULL, tcp_serv_beacon, NULL);
   pthread_join(timer, NULL);
    if(tcp_serv_beacon_activate) pthread_join(tcp_serv_beacon_thread, NULL);
     
    for(;;)
    {
        printf("inizio del ciclo ...\n");
        //clear the socket set
        FD_ZERO(&readfds);
  
        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
         
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
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
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
        }
          
        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
              
            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                      
                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }
                //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end of the data read

                    for(int n =0;n<sizeof(buffer);n++) buffer[n] =buffer[n] +2;
                    buffer[valread] = '\0';
                    send(sd , buffer , strlen(buffer) , 0 );
                }
            }
        }
    }
      
    return 0;
}
