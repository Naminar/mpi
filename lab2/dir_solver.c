#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "inc/param.h"

int main(int argc, char** argv){

    double box_eps = epsilon/nbox; 
    unsigned points_inside = npoints;
    double  delta       = 1,
            hight       = 0,
            box_square  = 0,
            square_prev = 0,
            square      = 0;

    double x_min;
    double x_max;
    double h;

    time_t begin = clock();
    for (unsigned box_ind = 0; box_ind < nbox; box_ind++){
        x_min = x_min_range + box_ind*(x_max_range-x_min_range)/nbox;
        x_max = x_min_range + (box_ind + 1)*(x_max_range-x_min_range)/nbox;
        delta = 1;
        points_inside = npoints;
        for (int step = 0; delta > box_eps; points_inside *= 2) {
            h = (x_max-x_min)/points_inside;
            hight = 0;
            for (int i = 0; i < points_inside; i++)
                hight += func(x_min + h*(i+0.5));
            box_square = hight*h;
            if (points_inside > npoints)
                delta = fabs(box_square - square_prev)/(3.0);
            square_prev = box_square;
        }
        square += box_square;
    }
    time_t end = clock();

    printf("square is %lf, epsilon is %lf\n", square, epsilon);
    printf("%lf\n",  (double)(end - begin) / CLOCKS_PER_SEC);

    return 0;
}