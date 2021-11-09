// Drew Cooper B00811386 ASN4
#include <stdio.h>
#include <stdlib.h>
#include "asn4.h"

int main(){

    char taskName;
    int taskNumber;
    int taskArrivalTime;
    int taskBurstTime;

printf("hello go fuckyourself");

    FILE* fp = fopen("TaskSpec.txt", "r");

    if (fp == NULL){
        printf("Error opening file\n");
        return 1;
    }

    fclose(fp);



    return 0;
}
