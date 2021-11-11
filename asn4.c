// Drew Cooper B00811386 ASN4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asn4.h"

void FCFS(Queue* queue);

int main(){

    char taskName;
    int taskNumber;
    int taskArrivalTime;
    int taskBurstTime;
    char inputLine[10];
    char charParser[4];
    printf("\nhello go fuckyourself\n");

    FILE* fp = fopen("TaskSpec.txt", "r");

    if (fp == NULL){
        printf("Error opening file\n");
        return 1;
    }

    while(fgets(inputLine, sizeof(inputLine), fp) != NULL){
        printf("%s\n", inputLine);    
        charParser[0]= strtok(inputLine, "T,\n");
        taskNumber = atoi(strtok(NULL, "T,\n"));
        taskArrivalTime = atoi(strtok(NULL, "T,\n"));
        taskBurstTime = atoi(strtok(NULL, "T,\n"));    
        printf("%s %d %d %d\n", charParser[0], taskNumber, taskArrivalTime, taskBurstTime);
    }

    fclose(fp);

    return 0;
}

// imitates the FCFS algorithm used in CPU scheduling 
