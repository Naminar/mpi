
#include "solver.h"

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

void free_queue(Queue* qu){
    RangeBox* next_task = NULL;
    RangeBox* current_task = qu->head;  
    
    while(current_task){
        next_task = current_task->p_nex_box;
        free(current_task);
        current_task = next_task;
    }
    free(qu);
    return;
}