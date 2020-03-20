#include <gtk/gtk.h>

GtkWidget *gtk_entry_new( void );
GtkWidget *entry,  *label, *main_box, *window, *button, *button2, *window, *millis_b, *idttlm_b, *num, *RSSI_b, *milis_t, *idtlm_t, *RSSI_t, *box, *boxx, *box_u;
//GtkBox *box, *boxx;
static void enter_callback( GtkWidget *widget, GtkWidget *entry )
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  printf ("Entry contents: %s\n", entry_text);
}

static void print_hello (GtkWidget *widget, gpointer   data)
{
  g_print ("Heuuul\n");
    char a[30];
    int b = 124;
    sprintf(a,"%d",b);
    gtk_entry_set_text (GTK_ENTRY (entry),  a);
  }

static void layout() {
    box = gtk_box_new (GTK_ORIENTATION_VERTICAL,5);
    box_u = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,5);
    
    button = gtk_button_new_with_label ("gtk_box_pack");
    g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
    gtk_box_pack_start (GTK_BOX (box_u), button, FALSE, FALSE,0);
    gtk_widget_show (button);

    button2 = gtk_button_new_with_label ("button 2");
    g_signal_connect (button2, "clicked", G_CALLBACK (print_hello), NULL);
    gtk_box_pack_start (GTK_BOX (box_u), button2, FALSE, FALSE,0);
    gtk_widget_show (button2);
    
    gtk_box_set_homogeneous ( GTK_BOX(box_u),TRUE);
    gtk_container_add (GTK_CONTAINER (box), box_u);
    
    boxx = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    label = gtk_label_new ("This is a Normal");
    gtk_box_pack_start (GTK_BOX (boxx), label, FALSE, FALSE,0);
    gtk_widget_show (label);
    entry = gtk_entry_new ();
    gtk_entry_set_width_chars (GTK_ENTRY (entry), 10);
        
          gtk_entry_set_text (GTK_ENTRY (entry), "hello");
        gtk_box_pack_start (GTK_BOX (boxx), entry, FALSE, FALSE,0);
    gtk_widget_show (entry);
    gtk_box_set_homogeneous ( GTK_BOX(boxx),TRUE);
    gtk_container_add (GTK_CONTAINER (box), boxx);
    gtk_container_set_border_width(GTK_CONTAINER(box),5);
    gtk_container_add (GTK_CONTAINER (window), box);
}

static void activate (GtkApplication *app, gpointer    user_data)
{
    gint tmp_pos;
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Interface");
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);

layout();
  gtk_widget_show_all (window);
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

