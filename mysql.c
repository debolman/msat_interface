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

void checkHostEntry(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
}

char *ip_from_name(char hostbuffer[]) {
    char *IPbuffer;
      struct hostent *host_entry;
      int hostname;
    
      host_entry = gethostbyname(hostbuffer);
      checkHostEntry(host_entry);
    
      IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
      //printf("Host IP: %s \n", IPbuffer);
    return IPbuffer;
}

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void write_wo_connection( int a, int b, int c, int d, int e, int f, int g, int h, int i, int j) {
    char buf[1024] = {};
    char query_string[] = {  "INSERT INTO carr VALUES (%llu,%d,%d,%d,%d,%d,%d,%d,%d,%d, %d)" };
        int tim = time(NULL);
    j = j/1000;
        sprintf(buf, query_string,tim,a,b,c,d,e,f,g,h,i,j);
        if (mysql_query(con,buf)) finish_with_error(con);
}

void write_power (uint32_t a, float b, float c, float d, float e, float f) {
    char buf[1024] = {};
    char query_string[] = {  "INSERT INTO power_tab VALUES (%llu,%d,%f,%f,%f,%f,%f)" };
        sprintf(buf, query_string,(unsigned long)time(NULL),a,b,c,d,e,f);
        if (mysql_query(con,buf)) finish_with_error(con);
}

void write_position (uint32_t a, float b, float c, uint32_t d, float e, uint32_t f, uint32_t g, float h, float i, uint16_t l, uint16_t m, uint32_t n, int16_t o, int32_t p) {
    char buf[1024] = {};
    char query_string[] = {  "INSERT INTO pos_table VALUES (%llu,%d,%f,%f,%d,%f,%d,%d,%f,%f,%d,%d,%d,%d,%d)" };
        sprintf(buf, query_string,(unsigned long)time(NULL),a,b,c,d,e,f,g,h,i,l,m,n,o,p);
        if (mysql_query(con,buf)) finish_with_error(con);
}

void mysql_connection() {
    
    con = mysql_init(NULL);
     
     if (con == NULL)
     {
         fprintf(stderr, "%s\n", mysql_error(con));
         exit(1);
     }

     if (mysql_real_connect(con, ip_from_name("debolman.ns0.it"), "pos_user", "pos_pwd",
             "pos_db", 0, NULL, 0) == NULL)
     {
         finish_with_error(con);
     }
    
}


// Returns hostname for the local computer
void checkHostName(int hostname)
{
    if (hostname == -1)
    {
        perror("gethostname");
        exit(1);
    }
}
  
// Returns host information corresponding to host name
void checkHstEntry(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
}
  
// Converts space-delimited IPv4 addresses
// to dotted-decimal format
void checkIPbuffer(char *IPbuffer)
{
    if (NULL == IPbuffer)
    {
        perror("inet_ntoa");
        exit(1);
    }
}
  
// Driver code
char * hos(int n)
{
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;
  
    // To retrieve hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    checkHostName(hostname);
  
    // To retrieve host information
    host_entry = gethostbyname(hostbuffer);
    checkHstEntry(host_entry);
  
    IPbuffer = inet_ntoa(*((struct in_addr*)
                           host_entry->h_addr_list[n]));

  
    return IPbuffer;
}