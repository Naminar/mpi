#include "../inc/methods.h"

void solve_angle(int time_num, int axes_num, double(* u)[time_num][axes_num], double h, double tau){
    Node node;
    for (int t_ind = 0; t_ind < time_num - 1; t_ind += 1){
        for (int x_ind = 1; x_ind < axes_num - 1; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            node.u_k_m = (*u)[t_ind][x_ind];
            node.u_k_m_1 = (*u)[t_ind][x_ind-1];
            (*u)[t_ind+1][x_ind] = next_point_angle(node, h, tau);
        }
    }
    return;
}



void solve_four_point(int time_num, int axes_num, double(* u)[time_num][axes_num], double h, double tau){
    Node node;
    for (int t_ind = 0; t_ind < time_num - 1; t_ind += 1){
        for (int x_ind = 1; x_ind < axes_num - 1; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            node.u_k_m = (*u)[t_ind][x_ind];
            node.u_k_m_1 = (*u)[t_ind][x_ind-1];
            node.u_k_m__1 = (*u)[t_ind][x_ind+1];
            (*u)[t_ind+1][x_ind] = next_point_four_point(node, h, tau);
        }
    }
    return;
}



void solve_three_point(int time_num, int axes_num, double(* u)[time_num][axes_num], double h, double tau){
    Node node;
    for (int t_ind = 0; t_ind < time_num - 1; t_ind += 1){
        for (int x_ind = 1; x_ind < axes_num - 1; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            // node.u_k_m = (*u)[t_ind][x_ind];
            node.u_k_m_1 = (*u)[t_ind][x_ind-1];
            node.u_k_m__1 = (*u)[t_ind][x_ind+1];
            (*u)[t_ind+1][x_ind] = next_point_three_point(node, h, tau);
        }
    }
    return;
}



void solve_cross(int time_num, int axes_num, double(* u)[time_num][axes_num], double h, double tau){
    Node node;
    // int t_ind = 0;
    for (int t_ind = 0; t_ind < 1; t_ind += 1){
        for (int x_ind = 1; x_ind < axes_num - 1; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            node.u_k_m = (*u)[t_ind][x_ind];
            node.u_k_m_1 = (*u)[t_ind][x_ind-1];
            (*u)[t_ind+1][x_ind] = next_point_angle(node, h, tau);
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
            (*u)[t_ind+1][x_ind] = next_point_cross(node, h, tau);
        }
    }
    return;
}