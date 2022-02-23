#include <stdio.h>
#include <gtk/gtk.h>
#include "game.h"
#include "car.h"
#include "instruction.h"


static GdkPixbuf *create_pixbuf(const gchar * filename);
static void get_chosen();
static void set_chosen();
static void start(int argc, char *argv[]);
static void combo_selected_maps(GtkWidget *widget, gpointer window);
static void combo_select_color(GtkWidget *widget, gpointer val);
static void change_name(GtkWidget *widget, gpointer val);
static void combo_select_n(GtkWidget *widget, gpointer val);
static void combo_select_loops(GtkWidget *widget, gpointer val);
static void instruction_wind(int argc, char *argv[]);


struct Chosen {
    int n, loops;
    char map_name[100];
    char player[4][15];
    int color[4][3];
};

struct Chosen chosen;

myModule *my_modules;

int nums[4] = {0,1,2,3};
int colors[8][3];


int main(int argc, char *argv[]) {
    GdkPixbuf *icon;
    int a = 1;

    char label_name[8];
    my_modules = myModule_make();
    get_chosen();
    gtk_init (&argc, &argv);

    GtkWidget *window_tmp = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    if(window_tmp == NULL) {
        g_print("WOOPS\n");
        return 0;
    }
    (my_modules -> window) = window_tmp;
    gtk_window_set_title(GTK_WINDOW(my_modules -> window),
        "Wyścigi Samochodowe");
    gtk_window_set_position(GTK_WINDOW(my_modules -> window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(my_modules -> window), 100);
    g_signal_connect(G_OBJECT(my_modules -> window),"destroy",
        G_CALLBACK(gtk_main_quit),NULL);

    icon = create_pixbuf("setup/Game_Icon.png");  
    gtk_window_set_icon(GTK_WINDOW(my_modules -> window), icon);
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(my_modules -> window), box1);
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_box_pack_start(GTK_BOX(box1), grid, TRUE, FALSE, 0);
    
    GtkWidget *button, *combo, *color_combo;

    button=gtk_button_new_with_label("Start");
    g_signal_connect(G_OBJECT(button), "clicked",
        G_CALLBACK(start), NULL);

    gtk_grid_attach(GTK_GRID(grid), button, 0,0,2,1);


    struct dirent *file;
    DIR *dir_path;
    char name[100];
    combo = gtk_combo_box_text_new();
    if((dir_path = opendir(MAPS_DIR))) {
        while(( file = readdir( dir_path ) ) ) {
            strcpy(name, file -> d_name);
            if(strncmp(name, ".", 1) != 0) {
                gtk_combo_box_text_append_text(
                    GTK_COMBO_BOX_TEXT(combo), name);
            }
        }
        closedir(dir_path);
    }
    else
         g_print( "! wywołując funkcję opendir(__) pojawił się błąd otwarcia strumienia dla danej ścieżki, może nie istnieje, lub podano ścieżkę pustą\n");
    
    button = gtk_label_new("mapa:");
    gtk_grid_attach(GTK_GRID(grid), button, 0,1,1,1);

    g_signal_connect(G_OBJECT(combo), "changed", 
        G_CALLBACK(combo_selected_maps), NULL);

    gtk_grid_attach(GTK_GRID(grid), combo, 1,1,1,1);


    button = gtk_label_new("liczba graczy:");
    gtk_grid_attach(GTK_GRID(grid), button, 0,2,1,1);

    combo = gtk_combo_box_text_new();
    for(int i = 1; i <= 4; i++) {
        name[0] = i + '0';
        name[1] = 0;
        strcpy(name, name); 
        gtk_combo_box_text_append_text(
            GTK_COMBO_BOX_TEXT(combo), name);
    }
    
    g_signal_connect(G_OBJECT(combo), "changed", 
        G_CALLBACK(combo_select_n), NULL);
    gtk_grid_attach(GTK_GRID(grid), combo, 1,2,1,1);


    button = gtk_label_new("liczba okrążeń:");
    gtk_grid_attach(GTK_GRID(grid), button, 0,3,1,1);


    combo = gtk_combo_box_text_new();
    for(int i = 1; i <= 9; i++) {
        name[0] = i + '0';
        name[1] = 0;
        strcpy(name, name); 
        gtk_combo_box_text_append_text(
            GTK_COMBO_BOX_TEXT(combo), name);
    }
    
    g_signal_connect(G_OBJECT(combo), "changed", 
        G_CALLBACK(combo_select_loops), NULL);
    gtk_grid_attach(GTK_GRID(grid), combo, 1,3,1,1);

    

    FILE *colors_file = fopen(COLORS_FILE, "r");
    GtkListStore *liststore;
    GtkCellRenderer *column;

    liststore = gtk_list_store_new(1, G_TYPE_STRING);
    for(int i = 0; i < N_COLORS; i++) {
        int j = 0, c = 0;
        while((c = getc(colors_file)) != ';') {
            name[j] = c;
            j++;
        }
    
        name[j] = 0;
        strcpy(name, name);
        gtk_list_store_insert_with_values(liststore, NULL, -1,
                                        0, name,
                                        -1);
        fscanf(colors_file, "%d;%d;%d",
            &colors[i][0], &colors[i][1], &colors[i][2]);
        c = getc(colors_file);
    }
    for(int k = 0; k < 4; k++) for(int i = 0; i < 3; i++) {
        chosen.color[k][i] = colors[k][i];
    }

    for(int i = 0; i < 4; i++) {
        color_combo = gtk_combo_box_new_with_model(
            GTK_TREE_MODEL(liststore));

        column = gtk_cell_renderer_text_new();
        gtk_cell_layout_pack_start(
            GTK_CELL_LAYOUT(color_combo), column, TRUE);

        gtk_cell_layout_set_attributes(
            GTK_CELL_LAYOUT(color_combo), column,
            "cell-background", 0,
            NULL);

        gtk_combo_box_set_active(
            GTK_COMBO_BOX(color_combo), i);


        g_signal_connect(G_OBJECT(color_combo), "changed", 
            G_CALLBACK(combo_select_color), &nums[i]);

        gtk_grid_attach(GTK_GRID(grid), color_combo, 4, i + 1,1,1);
    }


    button=gtk_button_new_with_label("Instrukcja");
    g_signal_connect(G_OBJECT(button), 
        "clicked", G_CALLBACK(instruction_wind), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 1,4,1,1);

    button=gtk_button_new_with_label("Exit");
    g_signal_connect(G_OBJECT(button), 
        "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0,4,1,1);



    button = gtk_label_new("gracz");
    gtk_grid_attach(GTK_GRID(grid), button, 2,0,1,1);

    for(int i = 1; i <= 4; i++) {
        sprintf(label_name, "player%d", i);
        button = gtk_label_new(label_name);
        gtk_grid_attach(GTK_GRID(grid), button, 2,i,1,1);
    }

    button = gtk_label_new("nazwa");
    gtk_grid_attach(GTK_GRID(grid), button, 3,0,1,1);
    for(int i = 0; i < 4; i++) {
        button = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(button), chosen.player[i]);
        gtk_entry_set_max_length(GTK_ENTRY(button), LEN_PLAYER_NAME - 1);
        g_signal_connect(G_OBJECT(button), "changed", 
            G_CALLBACK(change_name), &nums[i]);
        gtk_grid_attach(GTK_GRID(grid), button, 3,i+1,1,1);
    }

    button = gtk_label_new("    kolor     ");
    gtk_grid_attach(GTK_GRID(grid), button, 4,0,1,1);

    gtk_widget_show_all(my_modules -> window);
    gtk_main ();
    return 0;
}


static void start(int argc, char *argv[]) {
    set_chosen();
    play_game(argc, argv);
    gtk_main_quit();
}

static void instruction_wind(int argc, char *argv[]) {
    instruction_show(argc, argv);
}

static void get_chosen() {
    FILE *setup_file = fopen(GAME_SETUP_INFO, "r");
    int n;
    fscanf(setup_file, "%d", &n);
    if (n > 4 || n < 1) n = 1;
    chosen.n = n;
    fscanf(setup_file, "%d", &n);
    if (n > 9 || n < 0) n = 1;
    chosen.loops = n;

    fscanf(setup_file, "%s", chosen.map_name);


    for(int i = 0; i < 4; i++) {
    fscanf(setup_file, "%s", chosen.player[i]);
    fscanf(setup_file, "%d;%d;%d",
        &chosen.color[i][0], &chosen.color[i][1], &chosen.color[i][2]);
    }
    fclose(setup_file);
}

static void set_chosen() {
    FILE *setup_file = fopen(GAME_SETUP_INFO, "w");
    char c;
    fprintf(setup_file, "%d\n", chosen.n);
    fprintf(setup_file, "%d\n", chosen.loops);
    fprintf(setup_file, "%s\n", chosen.map_name);

    for(int i = 0; i < 4; i++) {
        fprintf(setup_file, "%s\n", chosen.player[i]);
        fprintf(setup_file, "%d;%d;%d\n",
            chosen.color[i][0], chosen.color[i][1], chosen.color[i][2]);
    }
    fclose(setup_file);    
}

static void combo_selected_maps(GtkWidget *widget, gpointer window) {
    gchar *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
    strcpy(chosen.map_name, text);
    g_free(text);
}

static void combo_select_color(GtkWidget *widget, gpointer val) {
    int *player = val;
    int n_color = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    for(int i = 0; i < 3; i++) {
        chosen.color[*player][i] = colors[n_color][i];
    }
}

static void change_name(GtkWidget *widget, gpointer val) {
    int *player = val;
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(widget));
    strcpy(chosen.player[*player], name); 
}

static void combo_select_n(GtkWidget *widget, gpointer val) {
    gchar *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
    chosen.n = text[0] - '0';
    g_free(text);
}

static void combo_select_loops(GtkWidget *widget, gpointer val) {
    gchar *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
    chosen.loops = text[0] - '0';
    g_free(text);
}

static GdkPixbuf *create_pixbuf(const gchar * filename) {
    GdkPixbuf *pixbuf;
    GError *error = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if (!pixbuf) {
        
        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }

    return pixbuf;
}

