#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
// #include "queue.h"
#include "param.h"
// const double epsilon = 0.0000001;
// const unsigned nbox = 1;
// const double x_min_range = 0.001;
// const double x_max_range = 10.0;
// const unsigned npoints = 3;

// double func(double x){
//     return sin(1/x);
// }

typedef struct RangeBox RangeBox;

typedef struct RangeBox{
	double x_min;
	double x_max;
	RangeBox* p_nex_box;
} RangeBox;

typedef struct Queue{
	RangeBox* head;
	RangeBox* tail;
	RangeBox* current;
	double square;
} Queue;

RangeBox* get_box(Queue* qu){
	RangeBox* received_box =  qu->current;
	if (received_box)
		qu->current = received_box->p_nex_box;
	return received_box;
}

void push_box(Queue* qu, double x_min, double x_max){
	RangeBox* new_box = calloc(1, sizeof(RangeBox));
	if (!new_box)
		assert(0);
	new_box->x_min = x_min;
	new_box->x_max = x_max;
	new_box->p_nex_box = NULL;
	// qu->tail->p_nex_box = new_box;
	if (!qu->head && !qu->current && !qu->tail){
		// printf("here");
		qu->head = new_box;
		qu->current = new_box;
	} else {
		// printf("%p %p %p", qu->head, qu->current, qu->tail);
		qu->tail->p_nex_box = new_box;
	}
	qu->tail = new_box;
	return;
} 

Queue* init_queue(){
	Queue* task_queue = calloc(1, sizeof(Queue));
	task_queue->head = NULL;
	task_queue->current = NULL;
	task_queue->tail = NULL;
	task_queue->square = 0.0;
	return task_queue;
} 

pthread_mutex_t lock_get;
pthread_mutex_t lock_write;

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
		pthread_mutex_lock(&lock_get);
		RangeBox* task = get_box(qu);
		pthread_mutex_unlock(&lock_get);
			// printf("task for %lu is %p\n", pthread_self(), task);
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

			pthread_mutex_lock(&lock_write);
			qu->square += box_square;
			// printf("ps %lf\n", box_square);
			pthread_mutex_unlock(&lock_write);
		} else 
			break;
	}
	return NULL;
}

int main(int argc, char** argv){

    double box_eps = epsilon/nbox; 
    unsigned points_inside = npoints;
    double  delta       = 1,
            hight       = 0,
            box_square  = 0,
            square_prev = 0,
            square      = 0;

	// if (pthread_mutex_init(&lock, NULL) != 0) { 
	// 	printf("\n mutex init has failed\n"); 
	// 	return 1; 
	// } 

	assert(!pthread_mutex_init(&lock_get, NULL));
	assert(!pthread_mutex_init(&lock_write, NULL));

	Queue* task_queue = init_queue();
	for (unsigned box_ind = 0; box_ind < nbox; box_ind++){
        double x_min = x_min_range + box_ind*(x_max_range-x_min_range)/nbox;
        double x_max = x_min_range + (box_ind + 1)*(x_max_range-x_min_range)/nbox;
		push_box(task_queue, x_min, x_max);
	}

	unsigned thread_number = 3;
	pthread_t tid[thread_number];
	int error = 0, i = 0;  

	time_t begin_up = clock();

	for (int i = 0; i < thread_number; i++) {
		error = pthread_create(&tid[i], NULL, run_calculating, (void*)task_queue); 
		if (error != 0) 
			printf("\nThread can't be created :[%s]", 
				strerror(error)); 
		// i++; 
	} 

	time_t begin_down = clock();

	for (int i = 0; i < thread_number; i++)
       pthread_join(tid[i], NULL);
	time_t end = clock();
	// pthread_exit(NULL);
	pthread_mutex_destroy(&lock_get);
	pthread_mutex_destroy(&lock_write); 
    printf("square is %lf, epsilon is %lf\n", task_queue->square, epsilon);
    printf("%lf\n",  (double)(end - (begin_up + begin_down)/2) / CLOCKS_PER_SEC);

    return 0;
}
