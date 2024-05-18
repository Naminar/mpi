#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "inc/methods.h"

int const TIME_DIAP = 2;
int const AXES_DIAP = 1;
double const tau = 0.01;
double const h = 0.01;

double u_x(double x){
    return x*x*x/12; 
}

double u_t(double t){
    return t*t*t/12;
}

int main(int argc, char** argv){
    int time_num = TIME_DIAP/tau;
    int axes_num = AXES_DIAP/h;
    // double u[time_num][axes_num];
    
    double** u = malloc(sizeof(double*)*time_num);
    assert(u); 
    for (int t_ind = 0; t_ind < time_num; t_ind++){
        double* tmp_arr = (double*) calloc(axes_num, sizeof(double));
        assert(tmp_arr);
        u[t_ind] = tmp_arr;
    }

    Node node;

    FILE* dump;
    dump = fopen("direct_results.txt", "w+");

    time_t begin = clock();

    for (int t_ind = 0; t_ind < time_num; t_ind += 1)
        u[t_ind][0] = u_t(t_ind*tau);
    for (int x_ind = 0; x_ind < axes_num; x_ind += 1)
        u[0][x_ind] = u_x(x_ind*h);

    solve_cross(time_num, axes_num, u, h, tau);

    time_t end = clock();
    printf("%lf\n",  (double)(end - begin) / CLOCKS_PER_SEC);

    if (dump) {
        for (int t_ind = 0; t_ind < time_num - 1; t_ind += 1)
            fprintf(dump, "%lf ", t_ind*tau);
        fprintf(dump, "\n");
        
        for (int x_ind = 0; x_ind < axes_num - 1; x_ind += 1)
            fprintf(dump, "%lf ", x_ind*h);
        fprintf(dump, "\n");

        for (int t_ind = 0; t_ind < time_num - 1; t_ind += 1){
            for (int x_ind = 0; x_ind < axes_num - 1; x_ind += 1){
                fprintf(dump, "%lf ", u[t_ind][x_ind]);
            }
            fprintf(dump, "\n");
        }
    }
    fclose(dump);

    for (int t_ind = 0; t_ind < time_num; t_ind++){
        free(u[t_ind]);
    }
    free(u);

    return 0;
}