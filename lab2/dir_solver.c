#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

const double epsilon = 0.001;
const unsigned nbox = 10;
const double x_min_range = 0.001;
const double x_max_range = 10.0;
const unsigned npoints = 3;

double func(double x){
    return sin(1/x);
}

int main(int argc, char** argv){

    double box_eps = epsilon/nbox; 
    unsigned points_inside = npoints;
    double  delta       = 1,
            hight       = 0,
            box_square  = 0,
            square_prev = 0,
            square      = 0;

    time_t begin = clock();
    for (unsigned box_ind = 0; box_ind < nbox; box_ind++){
        double x_min = x_min_range + box_ind*(x_max_range-x_min_range)/nbox;
        double x_max = x_min_range + (box_ind + 1)*(x_max_range-x_min_range)/nbox;
        delta = 1;
        points_inside = npoints;
        for (int step = 0; delta > box_eps; points_inside *= 2) {
            double h = (x_max-x_min)/points_inside;
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