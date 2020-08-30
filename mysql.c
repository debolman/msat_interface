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

void mysql_connection() {
    
    con = mysql_init(NULL);
     
     if (con == NULL)
     {
         fprintf(stderr, "%s\n", mysql_error(con));
         exit(1);
     }

     if (mysql_real_connect(con, ip_from_name("debolman.ns0.it"), "interface", "interface",
             "interface", 0, NULL, 0) == NULL)
     {
         finish_with_error(con);
     }
    
}


void *log_thd() {
    //write_wo_connection(counter,timee());
    counter++;
    return 0;
}

void *mysql_log() {
    mysql_connection();
    unsigned long long before = 0;
    while(1) {
        if (unix_secs() - before > 199 ) {
        pthread_create(&log_thread, NULL, log_thd, NULL);
            before = unix_secs();
              }
        //usleep(10000);
    }
    mysql_close(con);
}
