#ifndef NODE_H
#define NODE_H

typedef struct Node{
    double t_k;
    double x_m;
    double u_k_m;
    double u_k_m_1; // uk (m-1) 
    double u_k_m__1; // uk (m+1)
    double u_k_1_m;
} Node;

#endif