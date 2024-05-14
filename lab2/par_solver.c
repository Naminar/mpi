
#include <pthread.h>
#include "solver.h"
#include "param.h"

// pthread_mutex_t lock_get;
// pthread_mutex_t lock_write;

pthread_spinlock_t lock_get;
pthread_spinlock_t lock_write;

void* run_calculating(void* queue){
	Queue* qu = (Queue*) queue;
	double box_eps = epsilon/nbox; 
    unsigned points_inside = npoints;
    double  delta       = 1,
            hight       = 0,
            box_square  = 0,
            square_prev = 0;
	double h = 0;
	
	while(1){
		pthread_spin_lock(&lock_get);
		RangeBox* task = get_box(qu);
		pthread_spin_unlock(&lock_get);
		if (task){
			delta = 1;
			points_inside = npoints;
			for (int step = 0; delta > box_eps; points_inside *= 2) {
				h = (task->x_max - task->x_min)/points_inside;
				hight = 0;
				for (int i = 0; i < points_inside; i++)
					hight += func(task->x_min + h*(i+0.5));
				box_square = hight*h;
				if (points_inside > npoints)
					delta = fabs(box_square - square_prev)/(3.0);
				square_prev = box_square;
			}

			pthread_spin_lock(&lock_write);
			qu->square += box_square;
			pthread_spin_unlock(&lock_write);
		} else 
			break;
	}
	return NULL;
}

int main(int argc, char** argv){
    double 	box_eps = epsilon/nbox; 
    unsigned 	points_inside = npoints,
				thread_number = 4;
    double  delta       = 1,
            hight       = 0,
            box_square  = 0,
            square_prev = 0,
            square      = 0;

	assert(!pthread_spin_init(&lock_get, PTHREAD_PROCESS_PRIVATE));
	assert(!pthread_spin_init(&lock_write, PTHREAD_PROCESS_PRIVATE));

	Queue* task_queue = init_queue();
	for (unsigned box_ind = 0; box_ind < nbox; box_ind++){
        double x_min = x_min_range + box_ind*(x_max_range-x_min_range)/nbox;
        double x_max = x_min_range + (box_ind + 1)*(x_max_range-x_min_range)/nbox;
		push_box(task_queue, x_min, x_max);
	}

	pthread_t tid[thread_number];
	int error = 0, i = 0;  

	time_t begin_up = clock();
	for (int i = 0; i < thread_number; i++){
		error = pthread_create(&tid[i], NULL, run_calculating, (void*)task_queue); 
		if (error != 0) 
			printf("\nThread can't be created :[%s]", 
				strerror(error)); 
	} 
	time_t begin_down = clock();
	
	for (int i = 0; i < thread_number; i++)
       pthread_join(tid[i], NULL);
	time_t end = clock();
	
	pthread_spin_destroy(&lock_get);
	pthread_spin_destroy(&lock_write); 
    
	printf("square is %lf, epsilon is %lf\n", task_queue->square, epsilon);
    printf("%lf\n",  (double)(end - (begin_up + begin_down)/2) / CLOCKS_PER_SEC);

	free_queue(task_queue);
    return 0;
}
