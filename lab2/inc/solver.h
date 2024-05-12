
#include "queue.h"

RangeBox* get_box(Queue* qu);
void push_box(Queue* qu, double x_min, double x_max);
Queue* init_queue();
void free_queue(Queue* qu);