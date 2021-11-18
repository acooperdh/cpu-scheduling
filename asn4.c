// Drew Cooper B00811386 ASN4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asn4.h"
#include <stdbool.h>
#define TASK_SIZE 8

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

// time quantum is 4ms
// if a new tasks arrives at the same time as a task that is old
// the new one goes first in the queue 
void round_robin(Queue* queue, FILE* fp){
    int time_quantum = 4;
    int current_time = 0;;
    Queue* finished_queue = queue_initialize(sizeof(int)*TASK_SIZE);
    int num_of_tasks = queue->size;
    fprintf(fp, "\nRR:\n");
    while(queue->size > 0){
        void* next_element = queue_remove_element(queue, 0);
        Task* task = (Task*)next_element;
        task->start_time = current_time;
        if (task->remaining <= time_quantum){
            current_time += task->remaining;
            task->end_time = current_time;
            task->wait_time += task->start_time - task->arrival;
            fprintf(fp,"T%d\t%d\t%d\n", task->number, task->start_time, task->end_time);
            queue_enqueue(finished_queue, task);
        }
        else{
            current_time += time_quantum;
            fprintf(fp,"T%d\t%d\t%d\n", task->number, task->start_time, current_time);
            task->wait_time -= time_quantum;
            task->remaining -= time_quantum;
            queue_enqueue(queue, task);
        }
    }
    // sorting finished task queue by task number 
    sort_by_task_number(finished_queue);
    // output wait times for each task 
    output_waiting_times(finished_queue, fp);
    
    return;
}
// non preemptive shortest job first 
int npsjf(Queue* queue, FILE* fp){
    fprintf(fp, "\nNPSJF:\n");
    int current_time = 0;
    int num_of_tasks = queue->size;
    void* first_task = queue_dequeue(queue);
    Task* task = (Task*)first_task;
    int first_burst = task->burst;
    task->start_time = 0;
    task->end_time = first_burst;
    Queue* ready_queue = queue_initialize(sizeof(int)*TASK_SIZE);
    queue_enqueue(ready_queue, first_task);
    Queue* finished_queue = queue_initialize(sizeof(int)*TASK_SIZE);
    while(ready_queue->size > 0){
        void* curr_task = queue_peek(ready_queue);
        Task* current_task = (Task*)curr_task;
        // if current task is finished, add to finished queue and remove from ready queue
        if (current_task->remaining == 0){
            void* finished_task = queue_dequeue(ready_queue);
            Task* finished_task_ptr = (Task*)finished_task;
            finished_task_ptr->end_time = current_time;
            queue_enqueue(finished_queue, finished_task_ptr);
        } //check arrival of new tasks, add to wait times of tasks already in the queue
        else{
            /*
            1. check if a new task has arrived at the current time
            2. if so then compare it to the tasks already in the queue and enter it where it belongs based on time remaining 
            if there is two tasks with the same amount of time remaining, then the older task takes prioirty 
            3. increase the wait time of all tasks in the ready queue
            4. increase the current time 

            */
           for(int i = 0; i < queue->size; i++){
                void* element = queue_get_element(queue, i);
                Task* task = (Task*)element;
                // new task has arrived
                if (task->arrival == current_time){
                    // now compare the time remaning of the new tasks with all current tasks in the queue including the current task running
                    for(int j = 0; j < ready_queue->size; j++){
                        void* element = queue_get_element(ready_queue, j);
                        Task* queued_task = (Task*)element;
                        // if the new task has a shorter time remaining then the task in this position, insert the new task in that position
                        if (queued_task->remaining > task->remaining){
                            queue_add_at(ready_queue, j, task);
                            break;
                        }else if(queued_task->remaining == task->remaining){
                            // if the new task has a shorter time remaining then the task in this position, insert the new task in that position
                            if (queued_task->number > task->number){
                                queue_add_at(ready_queue, j, task);
                                break;
                            }
                        }
                    }
                }

           }
        }
    }
    // after the first task is completed, the next tasks selected are according to burst time 
    // and not arrival time
    // for(int i = 1; i < queue->size ; i++){
    //     for(int j = 2; j < queue->size - 1; j++){
    //         void* first_element = queue_get_element(queue, j);
    //         void* second_element = queue_get_element(queue, j+1);
    //         if(((Task*)first_element)->burst > ((Task*)second_element)->burst && ((Task*)second_element)->arrival < first_burst){
    //             void* temp = queue_remove_element(queue, j);
    //             queue_enqueue(queue, temp);
    //         }
    //     }
    // }
    for(int i = 0; i < queue->size; i++){
        void* next_element = queue_get_element(queue, i);
        Task* task = (Task*)next_element;
        task->start_time = current_time;
        current_time += task->burst;
        task->end_time = current_time;
        task->wait_time = task->start_time - task->arrival;
        fprintf(fp,"T%d\t%d\t%d\n", task->number, task->start_time, task->end_time);
    }

    sort_by_task_number(queue);
    output_waiting_times(queue, fp);
    return 0;
}

int main(){

    int taskNumber;
    int taskArrivalTime;
    int taskBurstTime;
    char inputLine[10];

    FILE* fp = fopen("TaskSpec2.txt", "r");

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
    //FCFS implementationnt fcfs(inital_tasks);
    fcfs(inital_tasks, output_file);
    round_robin(rr_tasks, output_file);
    //npsjf(npsjf_tasks, output_file);
    //psjf(psjf_tasks, output_file);
    fclose(output_file);

    return 0;
}
