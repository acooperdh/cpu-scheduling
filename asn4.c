// Drew Cooper B00811386 ASN4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asn4.h"

#define TASK_SIZE 8

// first come first serve method working properly, needs to output to file
int fcfs(void* q){
    Queue* queue = (Queue*)q;
    printf("FCSC:\n");
    int current_time = 0;
    int num_of_tasks = queue->size;
    double average_wait_time;
    int total_wait_time = 0;
    for(int i = 0; i < queue->size; i++){
        void* next_element = queue_get_element(queue, i);
        Task* task = (Task*)next_element;
        task->start_time = current_time;
        current_time += task->burst;
        task->end_time = current_time;
        task->wait_time = task->start_time - task->arrival;
        total_wait_time += task->wait_time;
        printf("T%d\t%d\t%d\n", task->number, task->start_time, task->end_time);
    }
    average_wait_time = (double)total_wait_time / (double)queue->size;
    for(int i = 0; i < queue->size; i++){
        void* next_element = queue_get_element(queue, i);
        printf("Waiting Time T%d: %d\n", ((Task*)next_element)->number, ((Task*)next_element)->wait_time);
    }
    printf("Average Waiting Time: %.2f\n", average_wait_time); 
    return 0;
}

// time quantum is 4ms
// if a new tasks arrives at the same time as a task that is old
// the new one goes first in the queue 
int round_robin(void* q){
    int time_quantum = 4;
    int current_time = 0;;
    int total_wait_time = 0;
    double average_wait_time;
    Queue* queue = (Queue*)q;
    Queue* finished_queue = queue_initialize(sizeof(int)*TASK_SIZE);
    int num_of_tasks = queue->size;
    printf("RR:\n");
    while(queue->size > 0){
        void* next_element = queue_remove_element(queue, 0);
        Task* task = (Task*)next_element;
        task->start_time = current_time;
        if (task->remaining <= time_quantum){
            current_time += task->remaining;
            task->end_time = current_time;
            task->wait_time += task->start_time - task->arrival;
            printf("T%d\t%d\t%d\n", task->number, task->start_time, task->end_time);
            queue_enqueue(finished_queue, task);
        }
        else{
            current_time += time_quantum;
            printf("T%d\t%d\t%d\n", task->number, task->start_time, current_time);
            task->wait_time -= time_quantum;
            task->remaining -= time_quantum;
            queue_enqueue(queue, task);
        }
    }
    // sorting finished queue by task number 
    // REWORK IF POSSIBLE
    for(int i = 0; i < finished_queue->size; i++){
        for(int j = 0; j < finished_queue->size - 1; j++){
            void* first_element = queue_get_element(finished_queue, j);
            void* second_element = queue_get_element(finished_queue, j+1);
            if(((Task*)first_element)->number > ((Task*)second_element)->number){
                void* temp = queue_remove_element(finished_queue, j);
                queue_enqueue(finished_queue, temp);
            }
        }
    }

    for(int i = 0; i < finished_queue->size; i++){
        void* next_element = queue_get_element(finished_queue, i);
        Task* task = (Task*)next_element;
        total_wait_time += task->wait_time;
        printf("Waiting Time T%d: %d\n", task->number, task->wait_time);
    }
    average_wait_time = (double)total_wait_time / (double)finished_queue->size;
    printf("Average Waiting Time: %.2f\n", average_wait_time);
    return 0;
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
    while(fgets(inputLine, sizeof(inputLine), fp) != NULL){
        printf("%s\n", inputLine);    
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
    }
    for(int i = 0; i < inital_tasks->size; i++){
        void* next_element = queue_get_element(inital_tasks, i);
        Task* new_task = malloc(sizeof(Task));
        Task* old_task = (Task*)next_element;
        new_task->number = old_task->number;
        new_task->arrival = old_task->arrival;
        new_task->burst = old_task->burst;
        new_task->remaining = old_task->burst;
        new_task->wait_time = 0;
        queue_enqueue(rr_tasks, new_task);
    }
    //FCFS implementationnt fcfs(inital_tasks);
    fcfs((void*)inital_tasks);
    round_robin((void*)rr_tasks);
    fclose(fp);

    return 0;
}
