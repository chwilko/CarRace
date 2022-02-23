#ifndef CAR_H
#define CAR_H

#define SPEED 0.2
#define FRIC 0.001
#define LEN_PLAYER_NAME 15


typedef struct my_color RGB;
typedef struct instr instruction;
typedef struct car Car;

struct my_color {
    int R;
    int G;
    int B;
};// RGB

struct instr {
    double x;
    double y;
    double fric;
    int stop;
};// instruction;

struct car {
    instruction *instr;
    RGB *color;
    double x;
    double y;
    double m_x;
    double m_y;
    double alpha;
    int checkpoints;
    char name[LEN_PLAYER_NAME];
    cairo_t *cr;
};// Car;



Car *car_make(double x, double y, instruction *instr, RGB *color, char *name);
RGB *color_make(int R, int G, int B);
instruction *instruction_make();
int tick(Car *car, int a[], double fric, int checkpoint, int n_checkpoints);


#endif