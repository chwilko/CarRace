#ifndef GAME_h
#define GAME_h

#include "car.h"

#define BASIC_WIDTH 100
#define BASIC_HEIGHT 100
#define BASIC_CAR_SIZE 1
#define SCALAR 2
#define ROAD_DRAW_JUMP 4
#define FPS 50
#define CONTROLS_FILE "setup/controls.txt"
#define GAME_SETUP_INFO "setup/current_game_info.txt"
#define MAPS_DIR "setup/maps/"
#define COLORS_FILE "setup/colors.txt"
#define N_COLORS 8

typedef struct map_ map;
typedef struct cars_ myCars;
typedef struct modules_ myModule;


struct map_ {
    int n;
    int n_checkpoints;
    int *n_c_list;
    double *left_x;
    double *left_y;
    double *right_x;
    double *right_y;
    double **map;
    int **checkpoints;
};//map

struct cars_ {
    int n;
    Car **cars; 
};//myCars

struct modules_ {
    GtkWidget *window, *game_box, *score_grid;
    GtkWidget *drawing_area;
    int window_width, window_height;
    double scalar;
    cairo_surface_t *surf;
    map *road;
    int loops;
    myCars *my_cars;
    int close;
};//myModule


myModule *myModule_make();
void play_game(int argc, char *argv[]);

#endif
