
#ifndef PARAM_H
#define PARAM_H
#include <math.h>

const double epsilon = 0.001;
const unsigned nbox = 1000;
const double x_min_range = 0.001;
const double x_max_range = 100.0;
const unsigned npoints = 3;

double func(double x){
    return sin(1/x);
}

#endif