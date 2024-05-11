#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int const TIME_DIAP = 10;
int const AXES_DIAP = 10;
double const tau = 0.01;
double const h = 0.01;

double u_x(double x){
    return 0; 
}

double u_t(double t){
    return 0;
}

double func(float t, float x){
    return sin(x)*cos(t);
}

typedef struct Node{
    double t_k;
    double x_m;
    double u_k_m;
    double u_k_m_1; // uk (m-1) 
    double u_k_m__1; // uk (m+1)
    double u_k_1_m;
} Node;

double next_point_angle(Node node){
    printf("func: %lf, %lf, %lf", func(node.t_k, node.x_m), node.t_k, node.x_m);
    return ((func(node.t_k, node.x_m) - (node.u_k_m - node.u_k_m_1)/h)*tau
                + node.u_k_m);
};

void solve_angle(int time_num, int axes_num, double(* u)[time_num][axes_num]){
    Node node;
    for (int t_ind = 0; t_ind < time_num - 1; t_ind += 1){
        for (int x_ind = 1; x_ind < axes_num - 1; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            node.u_k_m = (*u)[t_ind][x_ind];
            node.u_k_m_1 = (*u)[t_ind][x_ind-1];
            (*u)[t_ind+1][x_ind] = next_point_angle(node);
        }
    }
    return;
}

double next_point_four_point(Node node){
    return (func(node.t_k, node.x_m) + 0.5*tau*(node.u_k_m__1 - 2*node.u_k_m + node.u_k_m_1)/(h*h) 
    - (node.u_k_m__1 - node.u_k_m_1)/(2*h))*tau + node.u_k_m;
};

void solve_four_point(int time_num, int axes_num, double(* u)[time_num][axes_num]){
    Node node;
    for (int t_ind = 0; t_ind < time_num - 1; t_ind += 1){
        for (int x_ind = 1; x_ind < axes_num - 1; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            node.u_k_m = (*u)[t_ind][x_ind];
            node.u_k_m_1 = (*u)[t_ind][x_ind-1];
            node.u_k_m__1 = (*u)[t_ind][x_ind+1];
            (*u)[t_ind+1][x_ind] = next_point_four_point(node);
        }
    }
    return;
}

double next_point_three_point(Node node){
    return (func(node.t_k, node.x_m) - (node.u_k_m__1 - node.u_k_m_1)/(2*h))*tau
    + 0.5*(node.u_k_m__1 + node.u_k_m_1);
};

void solve_three_point(int time_num, int axes_num, double(* u)[time_num][axes_num]){
    Node node;
    for (int t_ind = 0; t_ind < time_num - 1; t_ind += 1){
        for (int x_ind = 1; x_ind < axes_num - 1; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            // node.u_k_m = (*u)[t_ind][x_ind];
            node.u_k_m_1 = (*u)[t_ind][x_ind-1];
            node.u_k_m__1 = (*u)[t_ind][x_ind+1];
            (*u)[t_ind+1][x_ind] = next_point_three_point(node);
        }
    }
    return;
}

double next_point_cross(Node node){
    static int key = 0;
    if (key < 3)
        printf("%lf %lf %lf\n", func(node.t_k, node.x_m)*2*tau, (node.u_k_m__1 - node.u_k_m_1)*tau/h, node.u_k_1_m);
    key += 1;
    return (( func(node.t_k, node.x_m)*2*tau - (node.u_k_m__1 - node.u_k_m_1)*tau/(h))
                + node.u_k_1_m);
};

void solve_cross(int time_num, int axes_num, double(* u)[time_num][axes_num]){
    Node node;
    // int t_ind = 0;
    for (int t_ind = 0; t_ind < 1; t_ind += 1){
        for (int x_ind = 1; x_ind < axes_num - 1; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            node.u_k_m = (*u)[t_ind][x_ind];
            node.u_k_m_1 = (*u)[t_ind][x_ind-1];
            (*u)[t_ind+1][x_ind] = next_point_angle(node);
        }
    }

    for (int t_ind = 1; t_ind < time_num - 1; t_ind += 1){
        for (int x_ind = 1; x_ind < axes_num - 1; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            // node.u_k_m = (*u)[t_ind][x_ind];
            node.u_k_m_1 = (*u)[t_ind][x_ind-1];
            node.u_k_m__1 = (*u)[t_ind][x_ind+1];
            node.u_k_1_m = (*u)[t_ind-1][x_ind];
            (*u)[t_ind+1][x_ind] = next_point_cross(node);
        }
    }
    return;
}

int main(int argc, char** argv){
    int time_num = TIME_DIAP/tau;
    int axes_num = AXES_DIAP/h;
    double u[time_num][axes_num];
    
    Node node;

    FILE* dump;
    dump = fopen("angle.txt", "w+");
    
    time_t 
    for (int t_ind = 0; t_ind < time_num; t_ind += 1)
        u[t_ind][0] = u_t(t_ind*tau);
    for (int x_ind = 0; x_ind < axes_num; x_ind += 1)
        u[0][x_ind] = u_x(x_ind*h);

    // for (int t_ind = 0; t_ind < time_num - 1; t_ind += 1){
    //     for (int x_ind = 1; x_ind < axes_num - 1; x_ind += 1){
    //         node.t_k = t_ind*tau;
    //         node.x_m = x_ind*h;
    //         node.u_k_m = u[t_ind][x_ind];
    //         node.u_k_m_1 = u[t_ind][x_ind-1];
    //         u[t_ind+1][x_ind] = next_point_angle(node);
    //     }
    // }
    solve_cross(time_num, axes_num, &u);

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
    } else
        fclose(dump);

    return 0;
}