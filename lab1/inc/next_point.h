
#ifndef CROSS_H
#define CROSS_H

#include <stdio.h>
#include <math.h>
#include "node.h"

double next_point_angle(Node node, double h, double tau);
double next_point_four_point(Node node, double h, double tau);
double next_point_three_point(Node node, double h, double tau);
double next_point_cross(Node node, double h, double tau);

#endif