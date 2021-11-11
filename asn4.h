#ifndef __ASN4_HEADER__
#define __ASN4_HEADER__ 

#include "queue.h"

typedef struct _task{
    int number; 
    int arrival;
    int burst;
    int remaining;
    int wait_time;
} Task;

#endif
