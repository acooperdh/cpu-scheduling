Drew Cooper B00811386 ASN4 

This program reads in a list of tasks, inserts them into queues and then runs 
them through one of four CPU scheduling alogrithms. The algorithms used are 
    1. First Come First Serve (FCFS)
    2. Round Robin (RR)
    3. Non-Preemptive Shortest Job First
    4. Preemptive Shortest Job First
It runs all of the algorithms, and for each algorithm outputs the task execution
order, the waiting time for each task and the average waiting time for the algorithm.
It outputs this information to an output file called Output.txt
The tasks themselves are read in from a text file called TaskSpec.txt 

In order to generate the program and run it, 
simple use the command 
    make
in the command line. This will run the makefile which will create
a queue object file that will then be compiled with asn4.c file. 
It will then run asn4 (the name of the exe). To clean up the files created 
simply run 
    make clean

Required Files: 
    queue.c 
    queue.h
    asn4.c
    asn4.h
    TaskSpec.txt

The queue.c and queue.h used in this assignment are based on a queue structure & linked_list structure 
that I created and used in CSCI 2122 Labs in the winter of 2021. I rewrote the queue to combine the 
required linked_list functions and queue functions to make it easier 
to compile and debug. 