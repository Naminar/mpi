
#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

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

#endif 