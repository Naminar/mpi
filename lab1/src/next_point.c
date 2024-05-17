
#include "../inc/next_point.h"

double func(float t, float x){
    return t*x;
}

double next_point_angle(Node node, double h, double tau){
    // printf("func: %lf, %lf, %lf", func(node.t_k, node.x_m), node.t_k, node.x_m);
    return ((func(node.t_k, node.x_m) - (node.u_k_m - node.u_k_m_1)/h)*tau
                + node.u_k_m);
};

double next_point_four_point(Node node, double h, double tau){
    return (func(node.t_k, node.x_m) + 0.5*tau*(node.u_k_m__1 - 2*node.u_k_m + node.u_k_m_1)/(h*h) 
    - (node.u_k_m__1 - node.u_k_m_1)/(2*h))*tau + node.u_k_m;
};

double next_point_three_point(Node node, double h, double tau){
    return (func(node.t_k, node.x_m) - (node.u_k_m__1 - node.u_k_m_1)/(2*h))*tau
    + 0.5*(node.u_k_m__1 + node.u_k_m_1);
};

double next_point_cross(Node node, double h, double tau){
    static int key = 0;
    if (key < 3)
        // printf("%lf %lf %lf\n", func(node.t_k, node.x_m)*2*tau, (node.u_k_m__1 - node.u_k_m_1)*tau/h, node.u_k_1_m);
    key += 1;
    return (( func(node.t_k, node.x_m)*2*tau - (node.u_k_m__1 - node.u_k_m_1)*tau/(h))
                + node.u_k_1_m);
};