
#ifndef METH_H
#define METH_H

#include "next_point.h"

void solve_angle(int time_num, int axes_num, double** u, double h, double tau);
void solve_four_point(int time_num, int axes_num, double** u, double h, double tau);
void solve_three_point(int time_num, int axes_num, double** u, double h, double tau);
void solve_cross(int time_num, int axes_num, double** u, double h, double tau);

#endif