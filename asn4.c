// Drew Cooper B00811386 ASN4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asn4.h"



int fcfs(Queue* tasks){
    int totalTime = 0;
    int arrival;
    int burst;
    while(tasks->size > 0){
        Task *task = (Task*)queue_dequeue(tasks);
        arrival = (int)task->arrival;
        burst = (int)task->burst;
        printf("T%d\t%d\t%d\n", task->number, arrival, task->burst);
        totalTime += task->burst;
        free(task);
    }
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
    Queue *inital_tasks = queue_initialize(sizeof(char));
    Queue *tasks = queue_initialize(sizeof(char));
    while(fgets(inputLine, sizeof(inputLine), fp) != NULL){
        printf("%s\n", inputLine);    
        taskNumber = atoi(strtok(inputLine, "T ,\n"));
        taskArrivalTime = atoi(strtok(NULL, "T,\n"));
        taskBurstTime = atoi(strtok(NULL, "T,\n"));    
        printf("%d %d %d\n", taskNumber, taskArrivalTime, taskBurstTime);
        Task *task = malloc(sizeof(Task));
        task->number = taskNumber;
        task->arrival = taskArrivalTime;
        task->burst = taskBurstTime;
        task->remaining = task->burst;
        task->wait_time = 0;
        printf("%d\t%d\t%d\n", task->number, task->arrival, task->burst);
        queue_enqueue(inital_tasks, task);
    }
    //FCFS implementationnt fcfs(inital_tasks);
    fcfs(inital_tasks);

    fclose(fp);

    return 0;
}
