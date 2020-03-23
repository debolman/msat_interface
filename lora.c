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

void decode_tlm() {
    char a[30];
    sprintf(a,"%d",tlm.rssi);
    gtk_label_set_text(GTK_LABEL(rssi_v)  ,a);
    sprintf(a,"%d",tlm.snr);
    gtk_label_set_text(GTK_LABEL(snr_v)  ,a);
    sprintf(a,"%d",tlm.freq_er_hz);
    gtk_label_set_text(GTK_LABEL(freq_err_v)  ,a);
    tlm.milis_p = tlm.milis_p/1000;
    tlm.milis_r = tlm.milis_r/1000;
    sprintf(a,"%d",tlm.milis_r);
    gtk_label_set_text(GTK_LABEL(millis_v_r)  ,a);
    sprintf(a,"%d",tlm.milis_p);
    gtk_label_set_text(GTK_LABEL(millis_v_p)  ,a);
gtk_switch_set_state (GTK_SWITCH (switch_pa),  tlm.pwr_pa);
    gtk_range_set_value(GTK_RANGE(pwr_sca),tlm.pwr_db);
                        }
