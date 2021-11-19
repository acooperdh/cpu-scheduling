// Drew Cooper B00811386 ASN4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asn4.h"
#include <stdbool.h>

#define TASK_SIZE 8
#define TIME_QUANTUM 4

/* Function: sort_by_task_number
 * -----------------------------
 * Sorts a queue of Tasks in ascending order by task number
 * 
 * queue: queue of unsorted tasks
 * 
 * Returns: void
 */
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
    // checks that the queue has been sorted properly
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

/* Function: sort_by_remaining
 * ---------------------------
 * Sorts a queue of Tasks in ascending order by time remaining
 * 
 * queue: queue of unsorted tasks
 * 
 * Returns: void
 */
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
    // checks to ensure that the queue is in the correct order
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

/* Function: new_tasks_check
 * -------------------------
 * Determines if any task(s) that have arrived and adds them to ready queue
 * 
 * init_queue: queue of all tasks 
 * ready_queue: queue of tasks that have arrived & are waiting
 * current_time: the current time in the CPU
 * 
 * Returns: void
 */
void new_tasks_check(Queue* init_queue, Queue* ready_queue, int current_time){
    for(int i = 0; i < init_queue->size; i++){
        void* check_arr_ptr = queue_get_element(init_queue, i);
        Task* check_arrival = (Task*)check_arr_ptr;
        if (check_arrival->arrival == current_time){
            queue_enqueue(ready_queue, check_arrival);
        }
    }
    return;
}
/* Function: increase_wait_times
 * -----------------------------
 * Increases the wait times of all tasks in a queue
 * 
 * queue: queue of tasks 
 * 
 * Returns: void
 */
void increase_wait_times(Queue* queue){
    for(int i = 0; i < queue->size; i++){
        void* next_elem = queue_get_element(queue, i);
        Task* task = (Task*)next_elem;
        task->wait_time+=1;
    }
}

/* Function: output_waiting_times
 * ------------------------------
 * Outputs the waiting times & average waiting time of a queue of tasks 
 * to an output file
 * queue: queue of finished tasks 
 * fp: output file 
 * 
 * Returns: void
 */
void output_waiting_times(Queue* queue, FILE* fp){
    // sort the queue to output the wait times based on task number
    sort_by_task_number(queue);
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

/* Function: fcfs
 * ---------------
 * Uses the First Come First Serve scheduling algorithm to determine
 * the execution order, wait times and average waiting times of a 
 * queue of tasks. It outputs all of that information to an output file
 * 
 * queue: queue of tasks 
 * fp: output file
 * 
 * Returns: void
 */
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

/* Function: rr
 * ------------
 * Using the Round Robin scheduling algorithm to determine the execution order,
 * waiting times and average waiting times of a queue of tasks. All of this 
 * information is outputted to a text file.
 * 
 * queue: queue of tasks
 * fp: output file
 * 
 * Returns: void
 */
void rr(Queue* init_queue, FILE* fp){
    // local function varibales
    int current_time = 0;
    int counter = 0; //used to keep track of the time quantum
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
    
    while(finished_queue->size != num_of_tasks){
        counter+=1;
        current_time += 1;
        curr_task_arr[0]->remaining -= 1;
        if(curr_task_arr[0]->remaining <= 0){
            curr_task_arr[0]->end_time = current_time;
            curr_task_arr[0]->wait_time = curr_task_arr[0]->end_time - curr_task_arr[0]->arrival - curr_task_arr[0]->burst;
            fprintf(fp, "T%d\t%d\t%d\n", curr_task_arr[0]->number, curr_task_arr[0]->start_time, curr_task_arr[0]->end_time);
            queue_enqueue(finished_queue, curr_task_arr[0]);
            if (ready_queue->size == 0){
                break;
            }
            void* temp = queue_dequeue(ready_queue);
            curr_task_arr[0] = (Task*)temp;
            curr_task_arr[0]->start_time = current_time;
            counter = 0;
        }else{
           new_tasks_check(init_queue, ready_queue, current_time);

           /* if the time quantum has been reached, record the execution times
              queue in the ready queue, and set the first item in the ready queue to be 
              the new current task
           */
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

    // output the wait times & average wait time 
    output_waiting_times(finished_queue, fp);
    return;

}
/* Function: npsjf
 * ---------------
 * Using a Non-Preemptive Shortest Job First scheduling algorithm, calculates
 * the execution order, task waiting times and average waiting time and outputs 
 * this information to an output file
 * 
 * queue: queue of tasks
 * fp: output file
 * 
 * Returns: void
 */
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

    while(finished_queue->size != num_of_tasks){
        current_time += 1;
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
        }
        
        int prev_queue_size = ready_queue->size;
        // determines if there is any new tasks that have arrived
        new_tasks_check(init_queue, ready_queue, current_time);
        if (ready_queue->size > prev_queue_size){
            // sort the queue based on burst time
            sort_by_remaining(ready_queue);
        }
        // increment the wait times for all tasks in the ready queue
        increase_wait_times(ready_queue);
    }

    // output the wait times & average wait time 
    output_waiting_times(finished_queue, fp);
    return;
}
/* Function: psjf
 * ---------------
 * Using a Preemptive Shortest Job First scheduling algorithm, calculates
 * the execution order, task waiting times and average waiting time and outputs 
 * this information to an output file
 * 
 * queue: queue of tasks
 * fp: output file
 * 
 * Returns: void
 */
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
        }

        int prev_queue_size = ready_queue->size;
        // determine if any new tasks have arrived
        new_tasks_check(init_queue, ready_queue, current_time);
        // determines if a new task has a shorter burst then the current task's remaining time left 
        if (ready_queue->size > prev_queue_size){
            sort_by_remaining(ready_queue); // sort the queue now that the new task(s) have been added 
            void* first_task_ptr = queue_peek(ready_queue);
            Task* first_task = (Task*)first_task_ptr;
            // if the new task is at the front of the queue and has a shorter burst, swap it with the current task
            if (first_task->arrival == current_time && first_task->remaining < current_task_arr[0]->remaining){
                current_task_arr[0]->end_time = current_time;
                fprintf(fp, "T%d\t%d\t%d\n", current_task_arr[0]->number, current_task_arr[0]->start_time, current_task_arr[0]->end_time);
                queue_enqueue(ready_queue, current_task_arr[0]);
                void* temp = queue_dequeue(ready_queue);
                current_task_arr[0] = (Task*)temp;
                current_task_arr[0]->start_time = current_time;
                sort_by_remaining(ready_queue); // sort the ready queue again now that the current task has changed
            }
        }
        // incremenet the wait times for all tasks in the ready queue 
        increase_wait_times(ready_queue);
    }
    // output the wait times & average wait time 
    output_waiting_times(finished_queue, fp);
    return;
}

int main(){
    // temp variables that will be used to read in and set task params
    int taskNumber;
    int taskArrivalTime;
    int taskBurstTime;
    char inputLine[10];

    FILE* fp = fopen("TaskSpec2.txt", "r");

    if (fp == NULL){
        printf("Error opening file\n");
        return 1;
    }

    // initialize a queue for each of the scheduling algorithms 
    Queue *inital_tasks = queue_initialize(sizeof(int)*TASK_SIZE);
    Queue *rr_tasks = queue_initialize(sizeof(int)*TASK_SIZE);
    Queue *npsjf_tasks = queue_initialize(sizeof(int)*TASK_SIZE);
    Queue *psjf_tasks = queue_initialize(sizeof(int)*TASK_SIZE);

    // read in tasks & add them to the queues
    while(fgets(inputLine, sizeof(inputLine), fp) != NULL){
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
    
    // 4 CPU scheduling algorithms
    fcfs(inital_tasks, output_file);
    rr(rr_tasks, output_file);
    nspjf(npsjf_tasks, output_file);
    psjf(psjf_tasks, output_file);

    fclose(output_file);

    return 0;
}
