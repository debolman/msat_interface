#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT     6070
#define MAXLINE 1024 
  
// Driver code 
int main() { 
    int sockfd, listenfd, len; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    bzero(&servaddr, sizeof(servaddr)); 
  
    // Create a UDP Socket 
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);  
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
      
	len = sizeof(cliaddr); 
    int n = recvfrom(listenfd, buffer, sizeof(buffer), 
            0, (struct sockaddr*)&cliaddr,&len); //receive message from server 
    buffer[n] = '\0'; 
    puts(buffer);  
      
    return 0; 
} 