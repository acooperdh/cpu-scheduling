#ifndef __ASN4_HEADER__
#define __ASN4_HEADER__ 

#include "queue.h"

typedef struct _task{
    int id; 
    int priority;
    int burst;
} Task;

#endif
