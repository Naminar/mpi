#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
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
void do_dump(int time_num, int axes_num, double** u);

typedef struct Node{
    double t_k;
    double x_m;
    double u_k_m;
    double u_k_m_1; // uk (m-1) 
    double u_k_m__1; // uk (m+1)
    double u_k_1_m;
} Node;

double func(float t, float x){
    return sin(x)*cos(t);
}

double next_point_angle(Node node, double h, double tau){
    return ((func(node.t_k, node.x_m) - (node.u_k_m - node.u_k_m_1)/h)*tau
                + node.u_k_m);
};

double next_point_cross(Node node, double h, double tau){
    return (( func(node.t_k, node.x_m)*2*tau - (node.u_k_m__1 - node.u_k_m_1)*tau/(h))
                + node.u_k_1_m);
};

int main(int argc, char ** argv)
{
    int * array;
    int tag=1;
    int size;
    int rank;
    int time_num = TIME_DIAP/tau;
    int axes_num = AXES_DIAP/h;
    MPI_Status status;
    MPI_Init (&argc,&argv);
    MPI_Comm_size (MPI_COMM_WORLD,&size);
    MPI_Comm_rank (MPI_COMM_WORLD,&rank);
    int x_cell_range = axes_num/size;
    
    clock_t begin = clock();

    if (rank == 0)
    {
        double* array = NULL;
        double** u = (double**) calloc(time_num, sizeof(double*));
        
        for (int t_ind = 0; t_ind < time_num; t_ind += 1){
            array = (double*) calloc(axes_num*2, sizeof(double));
            if (!array)
                MPI_Abort (MPI_COMM_WORLD,1);
            u[t_ind] = array;
        }
        
        double** box = (double**) calloc(time_num, sizeof(double*));
        for (int t_ind = 0; t_ind < time_num; t_ind += 1){
            array = (double*) calloc(x_cell_range, sizeof(double));
            if (!array)
                MPI_Abort (MPI_COMM_WORLD,1);
            box[t_ind] = array;
        }

        // edge
        for (int t_ind = 0; t_ind < time_num; t_ind += 1)
            box[t_ind][0] = u_t(t_ind*tau);
        for (int x_ind = 0; x_ind < x_cell_range; x_ind += 1)
            box[0][x_ind] = u_x(x_ind*h); 

        Node node;
        // angle
        int t_ind = 0;
        for (int x_ind = 0; x_ind < x_cell_range; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            node.u_k_m = box[t_ind][x_ind];
            node.u_k_m_1 = box[t_ind][x_ind-1];
            box[t_ind+1][x_ind] = next_point_angle(node, h, tau);
        }
            MPI_Send(&box[t_ind+1][x_cell_range-1], 1, MPI_DOUBLE, 
                        rank+1, tag, MPI_COMM_WORLD);

        for (int t_ind = 1; t_ind < time_num - 1; t_ind += 1){
            for (int x_ind = 1; x_ind < x_cell_range - 1; x_ind += 1){
                node.t_k = t_ind*tau;
                node.x_m = x_ind*h;
                node.u_k_m_1 = box[t_ind][x_ind-1];
                node.u_k_m__1 = box[t_ind][x_ind+1];
                node.u_k_1_m = box[t_ind-1][x_ind];
                box[t_ind+1][x_ind] = next_point_cross(node, h, tau);
            }

            int x_ind = x_cell_range - 1;
            node.t_k = t_ind*tau;
            node.x_m = x_ind*h;
            node.u_k_m = box[t_ind][x_ind];
            node.u_k_m_1 = box[t_ind][x_ind-1];
            box[t_ind+1][x_ind] = next_point_angle(node, h, tau);

            MPI_Send(&box[t_ind+1][x_cell_range-1], 1, MPI_DOUBLE, 
                        rank+1, tag, MPI_COMM_WORLD);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double* buffer_recv = (double*) calloc(axes_num, sizeof(double));
        for (int layer = 0; layer < time_num; layer += 1){
            MPI_Allgather(box[layer],
                    x_cell_range,
                    MPI_DOUBLE,
                    u[layer],// buffer_recv, // u[layer],
                    x_cell_range,
                    MPI_DOUBLE,
                    MPI_COMM_WORLD);
        }
        clock_t end = clock();
        printf("%lf\n",  (double)(end - begin) / CLOCKS_PER_SEC);

        do_dump(time_num, axes_num, u);
    }
     
    if (rank != 0)
    {  
        double* array = NULL;
        double** box = (double**) calloc(time_num, sizeof(double*));
        for (int t_ind = 0; t_ind < time_num; t_ind += 1){
            array = (double*) calloc(x_cell_range, sizeof(double));
            if (!array)
                MPI_Abort (MPI_COMM_WORLD,1);
            box[t_ind] = array;
        }

        // edge
        for (int x_ind = 0; x_ind < x_cell_range; x_ind += 1){
            box[0][x_ind] = u_x((x_cell_range*rank + x_ind)*h);
        }

        Node node;
        // angle
        int t_ind = 0;
        for (int x_ind = 0; x_ind < x_cell_range; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = (x_cell_range*rank + x_ind)*h;
            node.u_k_m = box[t_ind][x_ind];
            node.u_k_m_1 = box[t_ind][x_ind-1];
            box[t_ind+1][x_ind] = next_point_angle(node, h, tau);
        }
        
        t_ind = 0;
        for (int x_ind = 0; x_ind < x_cell_range; x_ind += 1){
            node.t_k = t_ind*tau;
            node.x_m = (x_cell_range*rank + x_ind)*h;
            node.u_k_m = box[t_ind][x_ind];
            node.u_k_m_1 = box[t_ind][x_ind-1];
            box[t_ind+1][x_ind] = next_point_angle(node, h, tau);
        }

        if (rank < size - 1)
            MPI_Send(&box[t_ind+1][x_cell_range-1], 1, MPI_DOUBLE, 
                        rank+1, tag, MPI_COMM_WORLD);

        for (int t_ind = 1; t_ind < time_num - 1; t_ind += 1){
            MPI_Recv (&box[t_ind][0],1,MPI_DOUBLE,rank-1,tag,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int x_ind = 1; x_ind < x_cell_range - 1; x_ind += 1){
                node.t_k = t_ind*tau;
                node.x_m = (x_cell_range*rank + x_ind)*h;
                node.u_k_m_1 = box[t_ind][x_ind-1];
                node.u_k_m__1 = box[t_ind][x_ind+1];
                node.u_k_1_m = box[t_ind-1][x_ind];
                box[t_ind+1][x_ind] = next_point_cross(node, h, tau);
            }

            int x_ind = x_cell_range - 1;
            node.t_k = t_ind*tau;
            node.x_m = (x_cell_range*rank + x_ind)*h;
            node.u_k_m = box[t_ind][x_ind];
            node.u_k_m_1 = box[t_ind][x_ind-1];
            box[t_ind+1][x_ind] = next_point_angle(node, h, tau);
            
            if (rank < size - 1){
                MPI_Send(&box[t_ind+1][x_cell_range-1], 1, MPI_DOUBLE, 
                        rank+1, tag, MPI_COMM_WORLD);
            }
        }

        MPI_Barrier(MPI_COMM_WORLD); 
        double* buffer_recv = (double*) calloc(axes_num, sizeof(double));
        for (int layer = 0; layer < time_num; layer += 1){
            MPI_Allgather(box[layer],
                    x_cell_range,
                    MPI_DOUBLE,
                    buffer_recv,
                    x_cell_range,
                    MPI_DOUBLE,
                    MPI_COMM_WORLD);
        }

    }
    
    MPI_Finalize();
}


void do_dump(int time_num, int axes_num, double** u){
    FILE* dump;
    dump = fopen("parallel.txt", "w+");

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
}