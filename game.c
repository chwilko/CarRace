//https://www.reddit.com/r/GTK/comments/ao9kcz/how_to_handle_doubleclick_events_in_gtk3/
//https://stackoverflow.com/questions/56920970/in-gtk3-how-do-i-get-a-drawingarea-to-respond-keyboard-events
/*
gcc `pkg-config --cflags gtk+-3.0` -o example ex1.c car.c `pkg-config --libs gtk+-3.0` -lm
*/
//https://stackoverflow.com/questions/46480555/gtk-blinks-when-drawing
#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>
#include <string.h>
#include "car.h"
#include "game.h"



void play_game(int argc, char *argv[]);
static gboolean game_tick(void *arg);
static void my_gtk_quit(GtkWidget *widget, myModule *my_modules);
myModule *myModule_make();
static void make_cars(int controls[], myModule *my_modules);
static  myCars *new_myCars();
static  map *new_map(int n, int n_c);
static void conect_lines(int **map, double x2, double y2, double x1, double y1, int val);
static void set_map(char *map_file, double start_places[4][2], myModule *my_modules);
static void set_place(int n, int labels[], int values[], int key, int val);
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, myModule *my_modules);
static void draw_map(cairo_t *cr, myModule *my_modules);
static void draw_checkpoints(cairo_t *cr, myModule *my_modules);
static void draving_car(cairo_t *cr, Car *car, myModule *my_modules);
static void read_controls(char *name_file, int controls[]);
static gboolean pressed(GtkWidget *widget, GdkEventKey *event, myModule *my_modules);
static gboolean relased(GtkWidget *widget, GdkEventKey *event, myModule *my_modules);
static void labels_make(int k, myModule *my_modules);
static int check_place(int place, int else_val, int dim);

int controls[16];
int keybo[26];
RGB *colors[4];
char results[100];
int win, quit;
int finished[4];
int game_time = 0;
char *counting_down_format = "<span foreground=\"magenta\" size=\"xx-large\" weight=\"bold\">%s</span>";

char *markup;


GtkWidget *label_score[4];
GtkWidget *res_label, *counting_down_label;
GtkWidget *exit_label;
double fric[3] = {0.1, 0.75, 1};


void play_game(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    myModule *my_modules = myModule_make();
    my_modules -> window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    for(int i = 0; i < 26; keybo[i++] = 0);
    int n;

    for(int i = 0; i < 4; i++) {
        finished[i] = 0;
    }
    win = 0;
    quit = 0;
    game_time = 0;
    gtk_window_set_title(GTK_WINDOW(my_modules -> window),
        "Wyścigi Samochodowe");
    gtk_window_set_position(GTK_WINDOW(my_modules -> window),
        GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(my_modules -> window), 250);
    g_signal_connect(G_OBJECT(my_modules -> window), "destroy",
        G_CALLBACK(my_gtk_quit), my_modules);

	my_modules -> game_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    my_modules -> drawing_area = gtk_drawing_area_new();
    my_modules -> score_grid = gtk_grid_new();


    my_modules -> close = 0;
    make_cars(controls, my_modules);


	// GtkWidget *da_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
	// GtkWidget *sg_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);



    g_signal_connect(G_OBJECT(my_modules -> drawing_area), "draw", 
        G_CALLBACK(on_draw_event), my_modules); 

    gtk_widget_add_events(my_modules -> window,
        GDK_KEY_PRESS_MASK
        | GDK_KEY_RELEASE_MASK);
        
    g_signal_connect(my_modules -> window, "key-press-event", 
        G_CALLBACK(pressed), my_modules);
    g_signal_connect(my_modules -> window,"key-release-event",
        G_CALLBACK(relased), my_modules);

    gtk_grid_set_row_spacing(GTK_GRID(my_modules -> score_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(my_modules -> score_grid), 10);


    GtkWidget *label;
    counting_down_label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(my_modules -> score_grid), counting_down_label, 0,0,2,1);
    markup = g_markup_printf_escaped(counting_down_format, "3");
    gtk_label_set_markup(GTK_LABEL(counting_down_label), markup);
    g_free (markup);
    label = gtk_label_new("Gracz");
    gtk_grid_attach(GTK_GRID(my_modules -> score_grid), label, 0,1,1,1);
    label = gtk_label_new("____");
    gtk_grid_attach(GTK_GRID(my_modules -> score_grid), label, 0,2,1,1);
    label = gtk_label_new("Punkt kontrolny");
    gtk_grid_attach(GTK_GRID(my_modules -> score_grid), label, 1,1,1,1);
    label = gtk_label_new("_______________");
    gtk_grid_attach(GTK_GRID(my_modules -> score_grid), label, 1,2,1,1);
    strcpy(results, "IN GAME\n\n\n\n");
    res_label = gtk_label_new(results);
    gtk_grid_attach(GTK_GRID(my_modules -> score_grid), res_label, 0,6,2,1);
    exit_label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(my_modules -> score_grid), exit_label, 0,7,2,1);


    // GtkWidget *grid = gtk_grid_new();
    // gtk_grid_set_column_spacing(GTK_GRID(grid), 10);


    // gtk_box_pack_start(GTK_BOX(my_modules -> game_box), grid, TRUE, TRUE, 0);


    // gtk_grid_attach(GTK_GRID(grid), da_box, 0,0,1,10);
    // gtk_grid_attach(GTK_GRID(grid), sg_box, 11,0,1,1);


    // gtk_box_pack_start(GTK_BOX(sg_box), my_modules -> score_grid, TRUE, TRUE, 0);
    // gtk_box_pack_start(GTK_BOX(da_box), my_modules -> drawing_area, TRUE, TRUE, 0);
    // gtk_container_add(GTK_CONTAINER(da_box), my_modules -> drawing_area);

    // gtk_container_add(GTK_CONTAINER(my_modules -> game_box),
    //     sg_box);
    // gtk_container_add(GTK_CONTAINER(my_modules -> game_box),
    //     da_box);


    // gtk_box_pack_start(GTK_BOX(my_modules -> game_box), da_box, TRUE, TRUE, 0);
    // gtk_box_pack_start(GTK_BOX(my_modules -> game_box), sg_box, TRUE, TRUE, 0);
    
    // gtk_cell_area_box_pack_start(GTK_BOX(my_modules -> game_box),
    //     my_modules -> drawing_area, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(my_modules -> game_box),
        my_modules -> drawing_area, TRUE, TRUE, FALSE);
    // gtk_grid_attach(GTK_GRID(my_modules -> score_grid), my_modules -> drawing_area, 3,0,1,7);
    gtk_box_pack_start(GTK_BOX(my_modules -> game_box),
        my_modules -> score_grid, TRUE, TRUE, FALSE);    
    
    // gtk_container_add(GTK_CONTAINER(my_modules -> game_box),
    //     my_modules -> drawing_area);
    // gtk_container_add(GTK_CONTAINER(my_modules -> game_box),
    //     my_modules -> score_grid);
    // gtk_container_add(GTK_CONTAINER(my_modules -> game_box), sg_box);

    gtk_container_add(GTK_CONTAINER(my_modules -> window),
        my_modules -> game_box);


    gtk_widget_show_all(my_modules -> window);
    g_timeout_add(1000/FPS, game_tick, my_modules);
    gtk_main();
}


static gboolean game_tick(void *arg) {
    /*
    Funkcja odpowiedzialna za ciągłość czasu w grze. 
    */
    myModule *my_modules = arg;
    
    if(game_time <= 3 * FPS){
        if(game_time++ == FPS){
            markup = g_markup_printf_escaped(counting_down_format, "2");
            gtk_label_set_markup(GTK_LABEL(counting_down_label), markup);
            g_free (markup);
        return TRUE;
        }else if(game_time == 2 * FPS) {
            markup = g_markup_printf_escaped(counting_down_format, "1");
            gtk_label_set_markup(GTK_LABEL(counting_down_label), markup);
            g_free (markup);
            return TRUE;
        }else if(game_time == 3 * FPS) {
            markup = g_markup_printf_escaped(counting_down_format, "GO!!!");
            gtk_label_set_markup(GTK_LABEL(counting_down_label), markup);
            g_free (markup);
            return TRUE;
        }
    }

    if(my_modules -> close == 1) return FALSE;
    int check;
    char str[4];
    

    if(game_time > 3 * FPS) if((my_modules -> my_cars) -> cars[0] != NULL)
        for(int i = 0; i < (my_modules -> my_cars) -> n; i++) {

        check = tick((my_modules -> my_cars) -> cars[i], &keybo[4*i],
            ((my_modules -> road) -> map)[
                check_place((int)((((my_modules -> my_cars) -> cars[i]) -> x) / SCALAR), BASIC_HEIGHT - 1, 0)]
                [check_place((int)(((my_modules -> my_cars) -> cars[i]) -> y / SCALAR), BASIC_WIDTH - 1, 1)],
            (my_modules -> road) -> checkpoints[
                check_place((int)(((my_modules -> my_cars) -> cars[i]) -> x / SCALAR), BASIC_HEIGHT - 1, 0)]
                [check_place((int)(((my_modules -> my_cars) -> cars[i]) -> y / SCALAR), BASIC_WIDTH - 1, 1)],
            (my_modules -> road) -> n_checkpoints);

        if (check == 1) {
            sprintf(str, "%d", ((my_modules -> my_cars) -> cars[i]) -> checkpoints - 1);
            gtk_label_set_text(GTK_LABEL(label_score[i]), str);
        }

        if(((((my_modules -> my_cars) -> cars[i]) -> checkpoints - 1) / (my_modules -> road -> n_checkpoints))
            >= my_modules -> loops) {
                if(win == 0) strcpy(results, "");
                if(finished[i] == 1) continue;
                finished[i] = 1;
                win++;
                sprintf(&results[20* (win - 1)], "\n%d %17s", win, ((my_modules -> my_cars) -> cars[i]) -> name);
                gtk_label_set_text(GTK_LABEL(res_label), results);
                if(win == 1) {
                    gtk_label_set_text(GTK_LABEL(exit_label), "Press Esc to Exit");
                }
            }
    }
    
    gtk_widget_queue_draw(my_modules -> window);
    
    gtk_window_get_size(GTK_WINDOW(my_modules -> window), &my_modules -> window_width, &my_modules -> window_height);
    if(my_modules -> window_width * BASIC_HEIGHT < my_modules -> window_height * BASIC_WIDTH)
        my_modules -> scalar = (double)my_modules -> window_width / BASIC_WIDTH;
    else
        my_modules -> scalar = (double)my_modules -> window_height / BASIC_HEIGHT;

    return TRUE;
}

static void my_gtk_quit(GtkWidget *widget, myModule *my_modules) {
    /*
    Funkcja zwalniająca ze sterty samochodziki i wyłączająca gtk.
    */
    my_modules -> close = 1;
    game_tick(my_modules);
    gtk_window_close(GTK_WINDOW(my_modules -> window));
    return;
}

myModule *myModule_make() {
    myModule *MM;
    if((MM = (struct modules_ *)malloc(sizeof(struct modules_))) == NULL)
        return NULL;
    if(((MM -> window) = (GtkWidget *)malloc(sizeof(GtkWidget))) == NULL)
        return NULL;

    if(((MM -> game_box) = (GtkWidget *)malloc(sizeof(GtkWidget))) == NULL)
        return NULL;
    if(((MM -> drawing_area) = (GtkWidget *)malloc(sizeof(GtkWidget))) == NULL)
        return NULL;
    if(((MM -> score_grid) = (GtkWidget *)malloc(sizeof(GtkWidget))) == NULL)
        return NULL;
    (MM -> surf) = NULL;
    (MM -> road) = NULL;
    (MM -> my_cars) = new_myCars();
    return MM;
}

static void make_cars(int controls[], myModule *my_modules) {
    int n;
    double start_places[4][2];
    int colorRGB[3];
    char map_file[100], map_name[88], player_name[LEN_PLAYER_NAME];
    RGB *color;

    strcpy(map_file, MAPS_DIR);
    
    read_controls(CONTROLS_FILE, controls);

    FILE *setup_file = fopen(GAME_SETUP_INFO, "r");

    fscanf(setup_file, "%d", &n);
    if (n > 4 || n < 1) n = 1;
    (my_modules -> my_cars) -> n = n;

    fscanf(setup_file, "%d", &n);
    if (n > 9 || n < 0) n = 1;
    (my_modules -> loops) = n;

    fscanf(setup_file, "%s", map_name);
    strcat(map_file, map_name);
    set_map(map_file, start_places, my_modules);
    for(int i = 0; i < (my_modules -> my_cars) -> n; i++) {

        fscanf(setup_file, "%s\n", player_name);
        fscanf(setup_file, "%d;%d;%d", &colorRGB[0], &colorRGB[1], &colorRGB[2]);
        color = color_make(colorRGB[0], colorRGB[1], colorRGB[2]);

        (my_modules -> my_cars) -> cars[i] =
            car_make(start_places[i][0], start_places[i][1], 
            instruction_make(), color, player_name);
        labels_make(i, my_modules);
    }

	fclose(setup_file);
}

static myCars *new_myCars() {
    myCars *my_cars;
    if((my_cars = (struct cars_ *)malloc(sizeof(struct cars_))) == NULL)
        return NULL;
    if(((my_cars -> cars) = (Car **)calloc(4, sizeof(Car *))) == NULL)
        return NULL;
    (my_cars -> n) = 0;
    return my_cars;
}

static map *new_map(int n, int n_c) {
     map *road;
    if((road = (struct map_ *)malloc(sizeof(struct map_))) == NULL)
        return NULL;
    road -> n = n;
    road -> n_checkpoints = n_c;

    if((road -> left_x = (double *)calloc(n, sizeof(double))) == NULL)
        return NULL;
    if((road -> right_x = (double *)calloc(n, sizeof(double))) == NULL)
        return NULL;
    if((road -> left_y = (double *)calloc(n, sizeof(double))) == NULL)
        return NULL;
    if((road -> right_y = (double *)calloc(n, sizeof(double))) == NULL)
        return NULL;
    if((road -> map = (double **)calloc(BASIC_HEIGHT, sizeof(double *))) == NULL)
        return NULL;

    if((road -> n_c_list = (int *)calloc(n_c, sizeof(int))) == NULL)
        return NULL;

    for(int i = 0; i < BASIC_HEIGHT; i++) {
        if((road -> map[i] = (double *)calloc(BASIC_WIDTH, sizeof(double))) == NULL)
            return NULL;
    }    
    if((road -> checkpoints = (int **)calloc(BASIC_HEIGHT, sizeof(int *))) == NULL)
        return NULL;
    for(int i = 0; i < BASIC_HEIGHT; i++) {
        if((road -> checkpoints[i] = (int *)calloc(BASIC_WIDTH, sizeof(int))) == NULL)
            return NULL;
    }

    if((road -> checkpoints = (int **)calloc(BASIC_HEIGHT, sizeof(int *))) == NULL)
        return NULL;
    for(int i = 0; i < BASIC_HEIGHT; i++) {
        if((road -> checkpoints[i] = (int *)calloc(BASIC_WIDTH, sizeof(int))) == NULL)
            return NULL;
    }
    return road;
}

static void conect_lines(int **map, double x2, double y2, double x1, double y1, int val) {
    int step;
    if(x1 > x2) {
        x1 = x1 + x2;
        y1 = y1 + y2;
        x2 = x1 - x2;
        y2 = y1 - y2;
        x1 = x1 - x2;
        y1 = y1 - y2;
    }
    if(y1 < y2) step = 1;
    else step = -1;
    if(x1 - x2 == 0) {
        for(int j = y1; j != (int)y2; j += step) {
            map[(int)(x1 / SCALAR) ][(int)(j / SCALAR)] = val;
        }
        map[(int)(x1 / SCALAR)][(int)(y2 / SCALAR)] = val;
    } else {
        double a = (y2 - y1) / (x2 - x1);
        double b = y2 - a * x2;

        for(int k = 0; (int)(x2-x1) >= k-1; k++) {// left x i right x sa nieuje
            for(int j = (int)(a * (x1 + k) + b);
                j != (int)(a * (x1 + k + 1) + b)
                    && (double)step * ((double)j - y1) >= 0
                    && (double)step * (y2 - (double)j) >= 0;
                j += step) {
                map[(int)((x1 + k) / SCALAR)][(int)(j / SCALAR)] = val;
            }
            map[(int)((x1 + k) / SCALAR)][(int)(y2 / SCALAR)] = val;
        }
    }
}

static void set_map(char *map_file,
    double start_places[4][2], myModule *my_modules) {
    /*
    Funkcja sczytuje mape z pliku i wrzuca ją do macierzy.
    */
    FILE *road_file = fopen(map_file, "r");
    char c;

    int n, n_c, loops;
    double d;
    double tmp, scalar, a, b, x1, x2, y1, y2;
    while((c = getc(road_file)) != '\n');
    fscanf(road_file, "%d", &n);

    fscanf(road_file, "%lf", &d);


    for(int i = 0; i < 4; i++) for(int j = 0; j < 2; j++)

        fscanf(road_file, "%lf", &start_places[i][j]);

    fscanf(road_file, "%d", &n_c);

    my_modules -> road = new_map(n, n_c);


    for(int i = 0; i < n_c; i++) {
        fscanf(road_file, "%d", &(my_modules -> road -> n_c_list[i]));
    }

    double x[n], y[n]; 
    for(int i = 0; i < n; i++) {
        fscanf(road_file, "%lf %lf", &x[i], &y[i]);
    }
    fclose(road_file);
    for(int i = 0; i < BASIC_HEIGHT; i++) {
        for(int j = 0; j < BASIC_WIDTH; j++) {
            my_modules -> road->map[i][j] = fric[1];
        }
    }
    
    for(int i = 0; i < BASIC_HEIGHT; i++) {
        for(int j = 0; j < BASIC_WIDTH; j++) {
            my_modules -> road->checkpoints[i][j] = 0;
        }
    }

    for(int i = 0; i < n - 1; i++) {
        if(y[i+1] - y[i] == 0) {
            my_modules -> road->left_x[i] = x[i];
            my_modules -> road->right_x[i] = x[i];
            my_modules -> road->left_y[i] = y[i]+d;
            my_modules -> road->right_y[i] = y[i]-d;
        } else { 
            tmp = -(x[i+1] - x[i]) / (y[i+1] - y[i]);
            scalar = d / 2 / sqrt(1 + tmp * tmp);
            if(y[i+1] - y[i] < 0) scalar *= -1;
            my_modules -> road->left_x[i] = x[i] + 1 * scalar;
            my_modules -> road->right_x[i] = x[i] - 1 * scalar;
            my_modules -> road->left_y[i] = y[i]+ tmp * scalar;
            my_modules -> road->right_y[i] = y[i]- tmp * scalar;
        }
    }
    my_modules -> road->left_x[n - 1] = x[n - 1] + 1 * scalar;
    my_modules -> road->right_x[n - 1] = x[n - 1] - 1 * scalar;
    my_modules -> road->left_y[n - 1] = y[n - 1]+ tmp * scalar;
    my_modules -> road->right_y[n - 1] = y[n - 1]- tmp * scalar;

    //ustawianie mniejszego tarcia na drodze.
    for(int i = 0; i < n; i++) {
        my_modules -> road->map[(int)(my_modules -> road->left_x[i] / SCALAR)][(int)(my_modules -> road->left_y[i] / SCALAR)] = fric[0];
        my_modules -> road->map[(int)(my_modules -> road->right_x[i] / SCALAR)][(int)(my_modules -> road->right_y[i] / SCALAR)] = fric[0];
    }
    int ctr, last;
    for(int i = 0; i < BASIC_HEIGHT / SCALAR; i++) {
        ctr = 0;
        last = 0;
        for(int j = 0; j < BASIC_WIDTH / SCALAR; j++) {
            if(my_modules -> road->map[i][j] == fric[1]) {
                last = 0;
                if(ctr == 1) {
                    my_modules -> road->map[i][j] = fric[0];
                }
            } else if(my_modules -> road->map[i][j] == fric[0]) {
                if(j == 0 || last == 1) continue;
                ctr = 1 - ctr;
                last = 1;
            }
        }
    }
    /* czyszczenie trasy*/ 
    for(int i = 1; i < BASIC_HEIGHT / SCALAR - 2; i++) {
        for(int j = 0; j < BASIC_WIDTH / SCALAR; j++) {
            if(my_modules -> road->map[i-1][j] == fric[0]
                && my_modules -> road->map[i+1][j] == fric[0]) {
                    my_modules -> road->map[i][j] = fric[0];
                } else if(my_modules -> road->map[i-1][j] == fric[0]
                && my_modules -> road->map[i+2][j] == fric[0]) {
                    my_modules -> road->map[i][j] = fric[0];
                }
        }
    }
    for(int i = 1; i < BASIC_HEIGHT / SCALAR - 2; i++) {
        for(int j = 0; j < BASIC_WIDTH / SCALAR; j++) {
            if((my_modules -> road) -> map[i-1][j] == fric[1]
                && (my_modules -> road) -> map[i+1][j] == fric[1]) {
                    (my_modules -> road) -> map[i][j] = fric[1];
                } else if(my_modules -> road->map[i-1][j] == fric[1]
                && my_modules -> road->map[i+2][j] == fric[1]) {
                    my_modules -> road->map[i][j] = fric[1];
                }
        }
    }
    (my_modules -> road) -> map[BASIC_HEIGHT - 1][BASIC_WIDTH - 1] = fric[2];

    for(int i = 0; i < my_modules -> road -> n_checkpoints; i++) {
        conect_lines(my_modules -> road -> checkpoints,
            my_modules -> road->left_x[my_modules -> road -> n_c_list[i]],
            my_modules -> road->left_y[my_modules -> road -> n_c_list[i]],
            my_modules -> road->right_x[my_modules -> road -> n_c_list[i]],
            my_modules -> road->right_y[my_modules -> road -> n_c_list[i]], i+1);
        conect_lines(my_modules -> road -> checkpoints,
            my_modules -> road->left_x[my_modules -> road -> n_c_list[i]-2],
            my_modules -> road->left_y[my_modules -> road -> n_c_list[i]-2],
            my_modules -> road->right_x[my_modules -> road -> n_c_list[i]+2],
            my_modules -> road->right_y[my_modules -> road -> n_c_list[i]+2], i+1);
        conect_lines(my_modules -> road -> checkpoints,
            my_modules -> road->left_x[my_modules -> road -> n_c_list[i]+2],
            my_modules -> road->left_y[my_modules -> road -> n_c_list[i]+2],
            my_modules -> road->right_x[my_modules -> road -> n_c_list[i]-2],
            my_modules -> road->right_y[my_modules -> road -> n_c_list[i]-2], i+1);

    }
    //* NIE USUWAĆ, PRZYDAJE SIĘ DO DEBUGOWANIA może kiedyś okpakuje w osobną funkcję
    for(int i = 0; i < BASIC_HEIGHT / SCALAR; i++) {
        for(int j = 0; j < BASIC_WIDTH / SCALAR; j++) {
            if(my_modules -> road -> checkpoints[j][i] > 0) {
                printf("%d", my_modules -> road -> checkpoints[j][i]);
            } else {
                if(my_modules -> road->map[j][i] == fric[1]) {
                    printf("*");
                } else if(my_modules -> road->map[j][i] == fric[0]) {
                    printf(" ");
                } else {
                    printf("%2.0lf", my_modules -> road->map[j][i]);
                }
            }
        }
        printf("\n");
    }
}

static void set_place(int n, int labels[], int values[],
    int key, int val) {
    /*
    Funkcja do ustawiania czy przycisk jest wciśnięty, czy zwolniony.
    */    
    for(int i = 0; i < n; i++) {
        if (labels[i] == key) {
            values[i] = val;
            return;
        }
    }
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, myModule *my_modules) {
    cairo_t *cr_tmp;
    my_modules -> surf = cairo_get_target(cr);
    cr_tmp = cairo_create(my_modules -> surf);
    draw_map(cr_tmp, my_modules);
    cairo_stroke(cr_tmp);
    cr_tmp = cairo_create(my_modules -> surf);
    draw_checkpoints(cr_tmp, my_modules);
    cairo_stroke(cr_tmp);
    
    for(int i = 0; i < (my_modules -> my_cars) -> n; i++) {
        cr_tmp = cairo_create(my_modules -> surf);
        draving_car(cr_tmp, ((my_modules -> my_cars) -> cars[i]), my_modules);
        cairo_stroke(cr_tmp);
    }
}

static void draw_map(cairo_t *cr, myModule *my_modules) {
    cairo_set_line_width(cr, 3.0);
	cairo_set_source_rgb(cr, 0.45,0.45,0.45);
    cairo_move_to(cr, my_modules -> road -> left_x[1] * my_modules -> scalar, 
        my_modules -> road -> left_y[1] * my_modules -> scalar);
    for(int i = 2; i < my_modules -> road -> n; i+=ROAD_DRAW_JUMP) {
        cairo_line_to(cr, my_modules -> road -> left_x[i] * my_modules -> scalar,
            my_modules -> road -> left_y[i] * my_modules -> scalar);
    }
        cairo_line_to(cr, my_modules -> road -> left_x[1] * my_modules -> scalar,
            my_modules -> road -> left_y[1] * my_modules -> scalar);
    cairo_move_to(cr, my_modules -> road -> right_x[1] * my_modules -> scalar, 
        my_modules -> road -> right_y[1] * my_modules -> scalar);
    for(int i = 2; i < my_modules -> road -> n; i+=ROAD_DRAW_JUMP) {
        cairo_line_to(cr, my_modules -> road -> right_x[i] * my_modules -> scalar,
            my_modules -> road -> right_y[i] * my_modules -> scalar);
    }
    cairo_line_to(cr, my_modules -> road -> right_x[1] * my_modules -> scalar, 
        my_modules -> road -> right_y[1] * my_modules -> scalar);

}

static void draw_checkpoints(cairo_t *cr, myModule *my_modules) {
    cairo_set_line_width(cr, 1.0);
	cairo_set_source_rgb(cr, 0,1,1);
    cairo_t *cr_tmp;
    for(int i = 0; i < my_modules -> road -> n_checkpoints; i++) {
        cairo_move_to(cr, my_modules -> road -> left_x[
                my_modules -> road -> n_c_list[i]] * my_modules -> scalar, 
            my_modules -> road -> left_y[my_modules -> road -> n_c_list[i]] * my_modules -> scalar);
        cairo_line_to(cr,
            my_modules -> road -> right_x[
                my_modules -> road -> n_c_list[i]] * my_modules -> scalar, 
            my_modules -> road -> right_y[
                my_modules -> road -> n_c_list[i]] * my_modules -> scalar);
        if(i == 0) {
            cr_tmp = cr;
            cr = cairo_create(my_modules -> surf);
            cairo_set_source_rgb(cr, 0,0,1);
        } else if (i == 1) {
            cairo_stroke(cr);
            cr = cr_tmp;
        }
    }
}

static void draving_car(cairo_t *cr, Car *car, myModule *my_modules) {
    /*
    Funkcja rysująca auto
    */
    double alpha0, alpha, x, y;
    alpha = car -> alpha;
    alpha0 = alpha;
    cairo_set_line_width(cr, 1.0);
	cairo_set_source_rgb(cr, (car -> color) -> R,
        (car -> color) -> G, (car -> color) -> B);
    x = (car -> x) * (my_modules -> scalar);
    y = (car -> y) * (my_modules -> scalar);
	alpha -= M_PI / 6.0;
	double r = BASIC_CAR_SIZE * my_modules -> scalar; 
    cairo_move_to(cr, (x + r * sin(alpha)), (y + r * cos(alpha)));
    alpha += M_PI / 3.0;
    cairo_line_to(cr, (x + r * sin(alpha)), (y + r * cos(alpha)));
    alpha += 2 * M_PI / 3.0;
    cairo_line_to(cr, (x + r * sin(alpha)), (y + r * cos(alpha)));
    alpha += M_PI / 3.0;
    cairo_line_to(cr, (x + r * sin(alpha)), (y + r * cos(alpha)));
    alpha += 2 * M_PI / 3.0;
    cairo_line_to(cr, (x + r * sin(alpha)), (y + r * cos(alpha)));
	alpha = alpha0 + M_PI;
	r *= 1.2;
	alpha -= M_PI / 6.0;
    cairo_move_to(cr, (x + r * sin(alpha)), (y + r * cos(alpha)));
    alpha += M_PI / 12.0;
    cairo_line_to(cr, (x + r * sin(alpha)), (y + r * cos(alpha)));
	alpha += M_PI / 6.0;
    cairo_move_to(cr, (x + r * sin(alpha)), (y + r * cos(alpha)));
    alpha += M_PI / 12.0;
    cairo_line_to(cr, (x + r * sin(alpha)), (y + r * cos(alpha)));
}

static void read_controls(char *name_file, int controls[]) {
    /*
    Funkcja zczytuje z pliku, którymi przyciskami się steruje.
    */
    FILE *controls_file = fopen(name_file, "r");
    for(int i = 0; i < 16; i++) {
        fscanf(controls_file, "%d", &controls[i]);
    }
    fclose(controls_file);
}

static gboolean pressed(GtkWidget *widget, GdkEventKey *event,
    myModule *my_modules) {
    /*
    Efekt wciśnięcia przycisku na klawiaturze
    */
    set_place(16, controls, keybo, event -> keyval, 1);
    return TRUE;
}

static gboolean relased(GtkWidget *widget, GdkEventKey *event,
    myModule *my_modules) {
    /*
    Efekt puszczenia przycisku na klawiaturze
    */
    if(event -> keyval == 65307) if(win > 0) {
        my_gtk_quit(NULL, my_modules);
        return TRUE;
    }
    set_place(16, controls, keybo, event -> keyval, 0);

    return TRUE;
}

static void labels_make(int k, myModule *my_modules) {
    GtkWidget *label;
    label = gtk_label_new(((my_modules -> my_cars) -> cars[k]) -> name);
    gtk_grid_attach(GTK_GRID(my_modules -> score_grid), label, 0,k+3,1,1);
    label_score[k] = gtk_label_new("0");
    gtk_grid_attach(GTK_GRID(my_modules -> score_grid), label_score[k], 1,k+3,1,1);
}

static int check_place(int place, int else_val, int dim) {
    if(dim == 0)
        if(place <= 0 || place >= BASIC_HEIGHT) return else_val;
    else if(dim == 1)
        if(place <= 0 || place >= BASIC_WIDTH) return else_val;
    return place;
}

