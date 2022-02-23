#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cairo.h>
#include "car.h"


Car *car_make(double x, double y, instruction *instr, RGB *color, char *name);
RGB *color_make(int R, int G, int B);
instruction *instruction_make();
static void set_mv(Car *car, int a[]);
static void set_fric(Car *car, double fric);
static void instruction_count(Car *car);
static void car_move(Car *car);
int tick(Car *car, int a[], double fric, int checkpoint, int n_checkpoints);
static void show_car(Car *car);
static void free_car(Car *car);


Car *car_make(double x, double y,
    instruction *instr, RGB *color, char *name) {
    Car *car;
    if((car = (struct car *)malloc(sizeof(struct car))) == NULL)
        return NULL;
    car -> x = x;
    car -> y = y;
    car -> m_x = 0;
    car -> m_y = 0;
    car -> alpha = M_PI;
    car -> instr = instr;
    car -> color = color;
    car -> checkpoints = 0;
    for(int i = 0; i < LEN_PLAYER_NAME; i++) {
        (car -> name)[i] = name[i];
    }
    return car;
}

RGB *color_make(int R, int G, int B) {
    RGB *color;
    if((color = (struct my_color *)malloc(sizeof(struct my_color))) == NULL)
        return NULL;
    color -> R = R;
    color -> G = G;
    color -> B = B;
    return color;
}

instruction *instruction_make() {
    instruction *instr;
    if((instr = (struct instr *)malloc(sizeof(struct instr))) == NULL)
        return NULL;
    instr -> fric = 0;
    instr -> x = 0;
    instr -> y = 0;
    instr -> stop = 0;
    return instr;
}

static void set_mv(Car *car, int a[]) {
    /*
    Tablica a[] odpowiada czterem przyciskom sterowania.
    Kolejno góra, prawo, dół, lewo. 
    Jeżeli odpowiedni przycisk był wciśnięty,
    odpowiednia wartość w tablicy wynosi 1, w przeciwny wypadku 0.
    */
    if (a[0] + a[1] + a[2] + a[3] == 0) {
        (car -> instr) -> stop = 0;
        (car -> instr) -> x = 0;
        (car -> instr) -> y = 0;
        return;
    } else if (a[0] + a[1] + a[2] + a[3] == 1) {
        (car -> instr) -> stop = 0;
        if(a[0] == 1) {
            (car -> instr) -> x = 0;
            (car -> instr) -> y = -1;           
        } else if(a[1] == 1) {
            (car -> instr) -> x = 1;
            (car -> instr) -> y = 0;           
        } else if(a[2] == 1) {
            (car -> instr) -> x = 0;
            (car -> instr) -> y = 1;           
        } else if(a[3] == 1) {
            (car -> instr) -> x = -1;
            (car -> instr) -> y = 0;           
        }
        return;
    } else if (a[0] + a[1] + a[2] + a[3] == 2) {
        (car -> instr) -> stop = 0;
        if(a[0] + a[1] == 2) {
            (car -> instr) -> x = 0.707;
            (car -> instr) -> y = -0.707;
            return;
        }
        if(a[1] + a[2] == 2) {
            (car -> instr) -> x = 0.707;
            (car -> instr) -> y = 0.707;
            return;
        }
        if(a[2] + a[3] == 2) {
            (car -> instr) -> x = -0.707;
            (car -> instr) -> y = 0.707;
            return;
        }
        if(a[3] + a[0] == 2) {
            (car -> instr) -> x = -0.707;
            (car -> instr) -> y = -0.707;
            return;
        }
    }
    (car -> instr) -> stop = 1;
    (car -> instr) -> x = 0;
    (car -> instr) -> y = 0;
}

static void set_fric(Car *car, double fric) {
    (car -> instr) -> fric = fric;
}

static void instruction_count(Car *car) {
    double v, rel;
    car -> m_x += SPEED * (car -> instr) -> x;
    car -> m_y += SPEED * (car -> instr) -> y;
    v = (car -> m_x) * (car -> m_x) + (car -> m_y) * (car -> m_y);
    v = sqrt(v);
    rel = FRIC + v * ((car -> instr) -> fric);

    if((car -> instr) -> stop == 1) rel += SPEED;

    if(rel > v) {
        rel = v;
    }

    if(v == 0)
        rel = 0;
    else
        rel = 1 - rel / v;
    car -> m_x *= rel;
    car -> m_y *= rel;
}

static void car_move(Car *car) {
    car -> x += car -> m_x;
    car -> y += car -> m_y;
    if(car -> m_y != 0) {
        car -> alpha = atan(car -> m_x / car -> m_y);
    }
    else {
        if(car -> m_x > 0)
            car -> alpha = -M_PI / 2;
        else if(car -> m_x < 0)
            car -> alpha = M_PI / 2;
    }
    if(car -> m_y > 0) {
        car -> alpha += M_PI;
    }
}

int tick(Car *car, int a[], double fric, int checkpoint, int n_checkpoints) {
    set_mv(car, a);
    set_fric(car, fric);
    instruction_count(car);
    car_move(car);
    if(checkpoint != 0) {
        if(checkpoint == ((car -> checkpoints)%n_checkpoints) + 1) {
            car -> checkpoints++;
            return 1;
        }
    }
    return 0;
}

static void show_car(Car *car) {
    printf("(%.3f, %.3f)\n", car -> x, car -> y);
    printf("<%.3f, %.3f>\n", car -> m_x, car -> m_y);
    printf("%.3f\n", car -> alpha);
}

static void free_car(Car *car) {
    free(car -> instr);
    free(car -> color);
    free(car);
}