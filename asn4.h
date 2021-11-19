#ifndef __ASN4_HEADER__
#define __ASN4_HEADER__ 

#include <stdio.h>
#include "queue.h"

typedef struct _task{
    int number; 
    int arrival;
    int burst;
    int remaining;
    int wait_time;
    int start_time;
    int end_time;
} Task;

void sort_by_task_number(Queue*);
void sort_by_remaining(Queue*);
void new_tasks_check(Queue*, Queue*, int);
void increase_wait_times(Queue*);
void output_waiting_times(Queue*, FILE*);
void fcfs(Queue*, FILE*);
void rr(Queue*, FILE*);
void npsjf(Queue*, FILE*);
void psjf(Queue*, FILE*);

#endif
