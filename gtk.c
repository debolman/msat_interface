#include "my_host.c"
static void print_hello (GtkWidget *widget, gpointer   data)
{
  g_print ("Heuuul\n");
    char a[30];
    int b = 124;
    sprintf(a,"%d",b);
    //gtk_label_set_text(GTK_LABEL(label3)  ,"hio");
    sprintf(a,"%d",udp_serv_port);
  }

static void activate_bool (GObject    *switcher,GParamSpec *pspec, gpointer    user_data) {
    bool *pun =user_data;
    if (gtk_switch_get_active (GTK_SWITCH (switcher)))  *pun = true;
    else *pun = false;
}

static void activate_pa (GObject    *switcher, GParamSpec *pspec, gpointer    user_data) {
    bool *pun =user_data;
            unsigned char buf[3];
    if (gtk_switch_get_active (GTK_SWITCH (switcher))) {

        buf[0] = 0x20;
        buf[1] = 1;
        buf[2] = 0x26;
        UDP_send(buf,3);
    }
    else {
        buf[0] = 0x20;
        buf[1] = 0;
        buf[2] = 0x26;
        UDP_send(buf,3);
    }
}

static void activate_serial_switch (GObject    *switcher, GParamSpec *pspec, gpointer    user_data) {
    bool *pun =user_data;
            
    if (gtk_switch_get_active (GTK_SWITCH (switcher))) {
        serial_initialize();
    }
    else {
        close(fd);
    }
}

static void activate_udp_switch (GObject    *switcher, GParamSpec *pspec, gpointer    user_data) {
    bool *pun =user_data;
            
    if (gtk_switch_get_active (GTK_SWITCH (switcher))) {
        socket_initialize();
        pthread_create(&udp_thread, NULL, UDP_listener, NULL);
    }
    else {
        close(sockfd);
    }
}

static void layout_rx() {
    frame =gtk_frame_new("Receiver");
       box = gtk_box_new (GTK_ORIENTATION_VERTICAL,5);
       gtk_container_set_border_width(GTK_CONTAINER(box),5);
       box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,5);
       gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
       label = gtk_label_new ("RSSI");
       gtk_widget_set_halign(label, GTK_ALIGN_END);
       gtk_container_add (GTK_CONTAINER (box_itm), label);
       rssi_v = gtk_label_new ("");
       gtk_container_add (GTK_CONTAINER (box_itm), rssi_v);
       gtk_container_add (GTK_CONTAINER (box), box_itm);
    
       box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,5);
       gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
       gtk_container_add (GTK_CONTAINER (box), box_itm);
       label = gtk_label_new ("SNR");
       gtk_widget_set_halign(label, GTK_ALIGN_END);
       gtk_container_add (GTK_CONTAINER (box_itm), label);
       snr_v = gtk_label_new ("");
       gtk_container_add (GTK_CONTAINER (box_itm), snr_v);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,5);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    label = gtk_label_new ("Freq err");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    freq_err_v = gtk_label_new ("");
    gtk_container_add (GTK_CONTAINER (box_itm), freq_err_v);
    
    
       gtk_container_add (GTK_CONTAINER (frame), box);
       gtk_container_add (GTK_CONTAINER (box_main), frame);
}

void vscale_moved (GtkRange *range,
              gpointer  user_data)
{
   GtkWidget *label = user_data;

   int pos = gtk_range_get_value (range);
    unsigned char buf[3];
    buf[0] = 0x10;
    buf[1] = pos;
    buf[2] = 0x26;
    UDP_send(buf,3);
}

static void layout_tx() {
    frame =gtk_frame_new("Trasmitter power");
       box = gtk_box_new (GTK_ORIENTATION_VERTICAL,5);
       gtk_container_set_border_width(GTK_CONTAINER(box),5);

       box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,5);
       gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
       label = gtk_label_new ("PA");
       gtk_container_add (GTK_CONTAINER (box_itm), label);
       switch_pa = gtk_switch_new ();
       g_signal_connect (GTK_SWITCH (switch_pa), "notify::active", G_CALLBACK (activate_pa), &tcp_raw);
       gtk_container_add (GTK_CONTAINER (box_itm), switch_pa);
       gtk_container_add (GTK_CONTAINER (box), box_itm);

       box_itm =gtk_box_new (GTK_ORIENTATION_HORIZONTAL,5);
       gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);

    pwr_sca = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0,20,1);
    g_signal_connect (pwr_sca,"value-changed",G_CALLBACK (vscale_moved),label);
        gtk_container_add (GTK_CONTAINER (box), pwr_sca);
       gtk_container_add (GTK_CONTAINER (box), box_itm);
    
       gtk_container_add (GTK_CONTAINER (frame), box);
       gtk_container_add (GTK_CONTAINER (box_main), frame);
}

static void layout_payload() {
    frame =gtk_frame_new("Payload");
       box = gtk_box_new (GTK_ORIENTATION_VERTICAL,5);
    gtk_container_set_border_width(GTK_CONTAINER(box),5);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,5);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("Millis");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    millis_v = gtk_label_new ("");
    gtk_container_add (GTK_CONTAINER (box_itm), millis_v);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
       gtk_container_add (GTK_CONTAINER (frame), box);
       gtk_container_add (GTK_CONTAINER (box_main), frame);
}



static void layout_config() {
    frame =gtk_frame_new("Configiration");
       box = gtk_box_new (GTK_ORIENTATION_VERTICAL,5);
    gtk_container_set_border_width(GTK_CONTAINER(box),5);
    int middle_space = 15;
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("UNIX");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    unix_time_l = gtk_label_new ("");
    gtk_container_add (GTK_CONTAINER (box_itm), unix_time_l);
    gtk_container_add (GTK_CONTAINER (box), box_itm);

    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("UDP act");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    udp_act_s = gtk_switch_new ();
    g_signal_connect (GTK_SWITCH (udp_act_s), "notify::active", G_CALLBACK (activate_udp_switch), &udp_raw);
    gtk_container_add (GTK_CONTAINER (box_itm), udp_act_s);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("UDP port");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    udp_serv_port_v = gtk_label_new ("");
    gtk_container_add (GTK_CONTAINER (box_itm), udp_serv_port_v);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("UDP raw");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    udp_raw_switch = gtk_switch_new ();
    g_signal_connect (GTK_SWITCH (udp_raw_switch), "notify::active", G_CALLBACK (activate_bool), &udp_raw);
    gtk_container_add (GTK_CONTAINER (box_itm), udp_raw_switch);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("TCP act");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    tcp_act_s = gtk_switch_new ();
    gtk_switch_set_active (GTK_SWITCH (tcp_act_s), FALSE);
    gtk_container_add (GTK_CONTAINER (box_itm), tcp_act_s);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("TCP port");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    tcp_serv_port_v = gtk_label_new ("");
    gtk_container_add (GTK_CONTAINER (box_itm), tcp_serv_port_v);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("TCP raw");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    tcp_raw_switch = gtk_switch_new ();
    g_signal_connect (GTK_SWITCH (tcp_raw_switch), "notify::active", G_CALLBACK (activate_bool), &tcp_raw);
    gtk_container_add (GTK_CONTAINER (box_itm), tcp_raw_switch);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("Serial act");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    serial_act_s = gtk_switch_new ();
    g_signal_connect (GTK_SWITCH (serial_act_s), "notify::active", G_CALLBACK (activate_serial_switch), &tcp_raw);
    gtk_container_add (GTK_CONTAINER (box_itm), serial_act_s);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("Serial size");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    ser_pkt_size_v = gtk_label_new ("");
    gtk_container_add (GTK_CONTAINER (box_itm), ser_pkt_size_v);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("Serial raw");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    serial_raw_switch = gtk_switch_new ();
    g_signal_connect (GTK_SWITCH (serial_raw_switch), "notify::active", G_CALLBACK (activate_bool), &serial_raw);
    gtk_container_add (GTK_CONTAINER (box_itm), serial_raw_switch);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("IP");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    ip_l = gtk_label_new ("");
    gtk_container_add (GTK_CONTAINER (box_itm), ip_l);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
    
    box_itm = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,middle_space);
    gtk_box_set_homogeneous(GTK_BOX(box_itm),TRUE);
    label = gtk_label_new ("IP");
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    gtk_container_add (GTK_CONTAINER (box_itm), label);
    ip_l2 = gtk_label_new ("");
    gtk_container_add (GTK_CONTAINER (box_itm), ip_l2);
    gtk_container_add (GTK_CONTAINER (box), box_itm);
       gtk_container_add (GTK_CONTAINER (frame), box);
       gtk_container_add (GTK_CONTAINER (box_main), frame);
}

static void layout() {
    box_main = gtk_box_new (GTK_ORIENTATION_VERTICAL,15);
    button = gtk_button_new_with_label ("test");
    g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
    gtk_box_pack_start (GTK_BOX (box_main), button, FALSE, FALSE,0);
    gtk_container_set_border_width(GTK_CONTAINER(box_main),10);
    gtk_container_add (GTK_CONTAINER (window), box_main);

}
void show_configuration() {
    char a[30];
    sprintf(a,"%d",udp_serv_port);
    gtk_label_set_text(GTK_LABEL(udp_serv_port_v)  ,a);
    sprintf(a,"%d",pkt_size);
    gtk_label_set_text(GTK_LABEL(ser_pkt_size_v)  ,a);
    sprintf(a,"%d",pkt_size);
    gtk_label_set_text(GTK_LABEL(ser_pkt_size_v)  ,a);
    sprintf(a,"%d",tcp_serv_port);
    gtk_label_set_text(GTK_LABEL(tcp_serv_port_v)  ,a);
    gtk_label_set_text(GTK_LABEL(ip_l)  ,hos(0));
    gtk_label_set_text(GTK_LABEL(ip_l2)  ,hos(1));
    gtk_switch_set_state(GTK_SWITCH(udp_raw_switch),serial_raw);
    gtk_switch_set_state(GTK_SWITCH(serial_raw_switch),udp_raw);
    gtk_switch_set_state(GTK_SWITCH(tcp_raw_switch),tcp_raw);
    gtk_switch_set_state(GTK_SWITCH(udp_act_s),udp_activate);
    gtk_switch_set_state(GTK_SWITCH(tcp_act_s),tcp_activation);
    gtk_switch_set_state(GTK_SWITCH(serial_act_s),serial_activate);
}

static void activate (GtkApplication *app, gpointer    user_data)
{
    gint tmp_pos;
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Interface");
  gtk_window_set_default_size (GTK_WINDOW (window), 180, 200);

layout();
    layout_config();
layout_rx();
layout_tx();
layout_payload();
  gtk_widget_show_all (window);
    show_configuration();
}

void GUI_act ()
{
  GtkApplication *app;
  int status;
  app = gtk_application_new ("org.gtk.dieoooooo", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app),0,0);
  g_object_unref (app);
 
}

