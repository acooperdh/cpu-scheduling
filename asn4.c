// Drew Cooper B00811386 ASN4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asn4.h"
#include <stdbool.h>
#define TASK_SIZE 8
#define TIME_QUANTUM 4

void sort_by_task_number(Queue* queue){
    for(int i = 0; i < queue->size; i++){
        for(int j = 0; j < queue->size - 1; j++){
            void* first_element = queue_get_element(queue, j);
            void* second_element = queue_get_element(queue, j+1);
            if(((Task*)first_element)->number > ((Task*)second_element)->number){
                void* temp = queue_remove_element(queue, j);
                queue_enqueue(queue, temp);
            }
        }
    }
    for(int i = 0; i < queue->size - 1; i++){
        void* current = queue_get_element(queue, i);
        void* next = queue_get_element(queue, i+1);
        Task* curr_task = (Task*)current;
        Task* next_task = (Task*)next;
        if(curr_task->number + 1 == next_task->number){
            continue;
        }else{
            sort_by_task_number(queue);
        }
        
    }
    return;
}

void sort_by_remaining(Queue* queue){
    for(int i = 0; i < queue->size; i++){
        for(int j = 0; j < queue->size - 1; j++){
            void* first_element = queue_get_element(queue, j);
            void* second_element = queue_get_element(queue, j+1);
            if(((Task*)first_element)->remaining > ((Task*)second_element)->remaining){
                void* temp = queue_remove_element(queue, j);
                queue_enqueue(queue, temp);
            }
        }
    }
    for(int i = 0; i < queue->size - 1; i++){
        void* current = queue_get_element(queue, i);
        void* next = queue_get_element(queue, i+1);
        Task* curr_task = (Task*)current;
        Task* next_task = (Task*)next;
        if(curr_task->remaining <= next_task->remaining){
            continue;
        }else{
            sort_by_remaining(queue);
        }
    }
    return;
}

// outputs waiting times for each task as well as returns the total amount of time waiting 
void output_waiting_times(Queue* queue, FILE* fp){
    double total_wait_time = 0;
    for(int i = 0; i < queue->size; i++){
        void* next_element = queue_get_element(queue, i);
        Task* task = (Task*)next_element;
        total_wait_time += task->wait_time;
        fprintf(fp, "Waiting Time T%d: %d\n", task->number, task->wait_time);
    }
    fprintf(fp, "Average Waiting Time: %.2f\n", total_wait_time/(double)queue->size);
    return; 
}

// first come first serve method working properly, needs to output to file
void fcfs(Queue* queue, FILE* fp){
    fprintf(fp,"FCSC:\n");
    int current_time = 0;
    for(int i = 0; i < queue->size; i++){
        void* next_element = queue_get_element(queue, i);
        Task* task = (Task*)next_element;
        task->start_time = current_time;
        current_time += task->burst;
        task->end_time = current_time;
        task->wait_time = task->start_time - task->arrival;
        fprintf(fp,"T%d\t%d\t%d\n", task->number, task->start_time, task->end_time);
    }
    
    output_waiting_times(queue, fp);
    
    return;
}

void print_queue(Queue* queue){
    for(int i = 0; i < queue->size; i++){
        void* next_element = queue_get_element(queue, i);
        Task* task = (Task*)next_element;
        printf("T%d\t%d\t%d\t%d , ", task->number, task->arrival, task->burst, task->remaining);
    }
}
// new round robin method -- works like a charm 
void rr(Queue* init_queue, FILE* fp){
    // local function varibales
    int current_time = 0;
    Queue* finished_queue = queue_initialize(sizeof(int)*TASK_SIZE);
    Queue* ready_queue = queue_initialize(sizeof(int)*TASK_SIZE);
    int num_of_tasks = init_queue->size;

    //current task variables
    void* void_curr_task = queue_dequeue(init_queue);
    Task* curr_task = (Task*)void_curr_task;
    curr_task->start_time = current_time;
    Task* curr_task_arr[1];
    curr_task_arr[0] = curr_task;
    fprintf(fp, "\nRR:\n");
    int counter = 0;
    while(finished_queue->size != num_of_tasks){
        counter+=1;
        current_time += 1;
        curr_task_arr[0]->remaining -= 1;
        if(curr_task_arr[0]->remaining <= 0){
            curr_task_arr[0]->end_time = current_time;
            fprintf(fp, "T%d\t%d\t%d\n", curr_task_arr[0]->number, curr_task_arr[0]->start_time, curr_task_arr[0]->end_time);
            curr_task_arr[0]->wait_time = curr_task_arr[0]->end_time - curr_task_arr[0]->arrival - curr_task_arr[0]->burst;
            queue_enqueue(finished_queue, curr_task_arr[0]);
            if (ready_queue->size == 0){
                break;
            }
            void* temp = queue_dequeue(ready_queue);
            curr_task_arr[0] = (Task*)temp;
            curr_task_arr[0]->start_time = current_time;
            counter = 0;
        }else{
            /* 1. Need to determine if any new tasks have arrived.
                2. if so then add them to the ready queue in the order that they arrive.
                3. if there is 2 tasks that arrive at the same time, the new one goes first
                4. if there is no new tasks then the loop continues on until the time quantum is reached.
            */
            for(int i = 0; i < init_queue->size; i++){
                void* check_arrival_ptr = queue_get_element(init_queue, i);
                Task* check_arrival = (Task*)check_arrival_ptr;
                if(check_arrival->arrival == current_time){
                    queue_enqueue(ready_queue, check_arrival);
                }
            }
           if(counter % 4 == 0){
                curr_task_arr[0]->end_time = current_time;
                Task* temp_task = curr_task_arr[0];
                fprintf(fp, "T%d\t%d\t%d\n", temp_task->number, temp_task->start_time, temp_task->end_time);
                queue_enqueue(ready_queue, temp_task);
                free(curr_task_arr[0]);
                void* temp = queue_dequeue(ready_queue);
                curr_task_arr[0] = (Task*)temp;
                curr_task_arr[0]->start_time = current_time;
                counter = 0;
           }
        }
    }

    // sorting finished task queue by task number
    sort_by_task_number(finished_queue);
    // output wait times for each task
    output_waiting_times(finished_queue, fp);
    return;

}
void nspjf(Queue* init_queue, FILE* fp){
    // local function variables 
    int current_time = 0;
    Queue* ready_queue = queue_initialize(sizeof(int)*TASK_SIZE);
    Queue* finished_queue = queue_initialize(sizeof(int)*TASK_SIZE);
    int num_of_tasks = init_queue->size;

    // current task variables 
    void* void_curr_task = queue_dequeue(init_queue);
    Task* current_task = (Task*)void_curr_task;
    current_task->start_time = current_time;
    Task* current_task_arr[1];
    current_task_arr[0] = current_task;
    
    fprintf(fp, "\nNPSJF:\n");
    int counter = 0; // not being used right now
    while(finished_queue->size != num_of_tasks){
        current_time += 1;
        counter+=1;
        current_task_arr[0]->remaining -= 1;
        // if the current task is finished, add it to the finished queue and get the next task in the queue
        if(current_task_arr[0]->remaining <= 0){
            current_task_arr[0]->end_time = current_time;
            fprintf(fp, "T%d\t%d\t%d\n", current_task_arr[0]->number, current_task_arr[0]->start_time, current_task_arr[0]->end_time);
            queue_enqueue(finished_queue, current_task_arr[0]);
            if (ready_queue->size == 0){
                break;
            }
            void* temp = queue_dequeue(ready_queue);
            current_task_arr[0] = (Task*)temp;
            current_task_arr[0]->start_time = current_time;
            counter = 0;
        }
            /*
                1. need to determine if new tasks have arrived
                2. if so, determine how many and ensure that they are entered into the queue based on
                    burst time
                3. if there is two or more tasks that arrive at the same time, then the one with the shorter
                burst / remaining should be the task placed higher in the queue
                4. if no new tasks, continue through the loop until there is new tasks 
            */
        int prev_queue_size = ready_queue->size;
        // determines if there is any new tasks that have arrived
        for(int i = 0; i < init_queue->size; i++){
            void* check_arr_ptr = queue_get_element(init_queue, i);
            Task* check_arrival = (Task*)check_arr_ptr;
            if (check_arrival->arrival == current_time){
                queue_enqueue(ready_queue, check_arrival);
            }
        }
        if (ready_queue->size > prev_queue_size){
            // sort the queue based on burst time
            sort_by_remaining(ready_queue);
        }
        // increment the wait times for all tasks in the ready queue
        for(int i = 0; i < ready_queue->size; i++){
            void* next_elem = queue_get_element(ready_queue, i);
            Task* task = (Task*)next_elem;
            task->wait_time+=1;
        }
    }

    sort_by_task_number(finished_queue);
    output_waiting_times(finished_queue, fp);
    return;
}

void psjf(Queue* init_queue, FILE* fp){
    // local function variables 
    int current_time = 0;
    Queue* ready_queue = queue_initialize(sizeof(int)*TASK_SIZE);
    Queue* finished_queue = queue_initialize(sizeof(int)*TASK_SIZE);
    int num_of_tasks = init_queue->size;

    // current task variables 
    void* void_curr_task = queue_dequeue(init_queue);
    Task* current_task = (Task*)void_curr_task;
    current_task->start_time = current_time;
    Task* current_task_arr[1];
    current_task_arr[0] = current_task;
    
    fprintf(fp, "\nPSJF:\n");
    int counter = 0;
    while(finished_queue->size != num_of_tasks){
        current_time += 1;
        current_task_arr[0]->remaining -= 1;
        if(current_task_arr[0]->remaining <= 0){
            current_task_arr[0]->end_time = current_time;
            fprintf(fp, "T%d\t%d\t%d\n", current_task_arr[0]->number, current_task_arr[0]->start_time, current_task_arr[0]->end_time);
            queue_enqueue(finished_queue, current_task_arr[0]);
            if (ready_queue->size == 0){
                break;
            }
            void* temp = queue_dequeue(ready_queue);
            current_task_arr[0] = (Task*)temp;
            current_task_arr[0]->start_time = current_time;
            counter = 0;
        }

        /*  1. determine if there is any new tasks that have arrived
            2. determine where the new tasks should go in the ready queue
            3. if the task at the front of the ready queue is new and has less time then the current task, 
            put the current task back into the queue(in it's proper place) and begin the shorter task that just arrived
            4. if there is no new tasks then simply continue with the current task until it is complete 
        */
        int prev_queue_size = ready_queue->size;
        // determine if any new tasks have arrived
        for(int i = 0; i < init_queue->size; i++){
            void* check_arr_ptr = queue_get_element(init_queue, i);
            Task* check_arrival = (Task*)check_arr_ptr;
            if (check_arrival->arrival == current_time){
                queue_enqueue(ready_queue, check_arrival);
            }
        }

        /*if there has been a new task(s) added, compare it's burst time to the time remaining on the first task
            1. sort the ready queue in order of remaining time left / burst (same thing at this point)
            2. peek the first task in the sorted ready queue
            3. if the first task arrival == current time, then we know a new task has the shortest burst in the queue currently
            4. if it's remaining is less then the current task, replace the current task with the new task and 
            enter the old current task into the ready queue in the proper place
        */
        if (ready_queue->size > prev_queue_size){
            sort_by_remaining(ready_queue);
            void* first_task_ptr = queue_peek(ready_queue);
            Task* first_task = (Task*)first_task_ptr;
            if (first_task->arrival == current_time && first_task->remaining < current_task_arr[0]->remaining){
                current_task_arr[0]->end_time = current_time;
                fprintf(fp, "T%d\t%d\t%d\n", current_task_arr[0]->number, current_task_arr[0]->start_time, current_task_arr[0]->end_time);
                queue_enqueue(ready_queue, current_task_arr[0]);
                void* temp = queue_dequeue(ready_queue);
                current_task_arr[0] = (Task*)temp;
                current_task_arr[0]->start_time = current_time;
                sort_by_remaining(ready_queue);
            }
        }
        // incremenet the wait times for all tasks in the ready queue 
        for(int i = 0; i < ready_queue->size; i++){
            void* next_elem = queue_get_element(ready_queue, i);
            Task* task = (Task*)next_elem;
            task->wait_time+=1;
        }
    }
    sort_by_task_number(finished_queue);
    output_waiting_times(finished_queue, fp);
    return;
}

int main(){

    int taskNumber;
    int taskArrivalTime;
    int taskBurstTime;
    char inputLine[10];

    FILE* fp = fopen("TaskSpec.txt", "r");

    if (fp == NULL){
        printf("Error opening file\n");
        return 1;
    }
    Queue *inital_tasks = queue_initialize(sizeof(int)*TASK_SIZE);
    Queue *rr_tasks = queue_initialize(sizeof(int)*TASK_SIZE);
    Queue *npsjf_tasks = queue_initialize(sizeof(int)*TASK_SIZE);
    Queue *psjf_tasks = queue_initialize(sizeof(int)*TASK_SIZE);
    while(fgets(inputLine, sizeof(inputLine), fp) != NULL){
        //printf("%s\n", inputLine);    
        taskNumber = atoi(strtok(inputLine, "T ,\n"));
        taskArrivalTime = atoi(strtok(NULL, "T,\n"));
        taskBurstTime = atoi(strtok(NULL, "T,\n"));    
        Task *task = malloc(sizeof(Task));
        task->number = taskNumber;
        task->arrival = taskArrivalTime;
        task->burst = taskBurstTime;
        task->remaining = task->burst;
        task->wait_time = 0;
        queue_enqueue(inital_tasks, task);
        queue_enqueue(rr_tasks, task);
        queue_enqueue(npsjf_tasks, task);
        queue_enqueue(psjf_tasks, task);
    }
    fclose(fp);
    
    FILE* output_file = fopen("Output.txt", "w");
    fcfs(inital_tasks, output_file);
    rr(rr_tasks, output_file);
    nspjf(npsjf_tasks, output_file);
    psjf(psjf_tasks, output_file);
    fclose(output_file);

    return 0;
}
