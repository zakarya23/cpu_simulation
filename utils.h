#ifndef ALLOCATEH
#define ALLOCATEH
#include "list.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define RUNNING "%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n"
#define RUNNING_PAR "%d,RUNNING,pid=%d.%d,remaining_time=%d,cpu=%d\n"
#define FINISHED "%d,FINISHED,pid=%d,proc_remaining=%d\n"
#define TURNAROUND "Turnaround time %d\n"
#define TIMEOVERHEAD "Time overhead %.2f %.2f\n"
#define MAKESPAN "Makespan %d\n"

int round_off(double *number); 
void print_performance(float turn_around, float max, float total, int total_time, int num_input); 
list_t *read_details(char *argv[], int *processors); 
#endif 