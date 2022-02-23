#include <string.h>
#include <gtk/gtk.h>
#include "instruction.h"

GtkWidget *window;

static void button_quit(GtkWidget *widget, gpointer point) {
    gtk_window_close(GTK_WINDOW(window));
}

void instruction_show(int argc,char *argv[]) {
    char text[1000];
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_title(GTK_WINDOW(window),"Instrukcja gry");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit), NULL);


    FILE *instr = fopen(INSTRUCTION_FILE, "r");
    int i = 0;
    char c;
    while((c = getc(instr)) != EOF) {
        text[i++] = c;
    }
    text[i] = ' ';
    
    GtkWidget *label = gtk_label_new(text);
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *button = gtk_button_new_with_label("Zamknij");
    g_signal_connect(G_OBJECT(button),"clicked", G_CALLBACK(button_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(button), 20);

   gtk_container_add(GTK_CONTAINER(box1), label);

    gtk_container_add(GTK_CONTAINER(box1), button);

    gtk_container_add(GTK_CONTAINER(window), box1);

    gtk_widget_show_all(window);
    gtk_main();
}