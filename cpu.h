#ifndef CPUH 
#define CPUH 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include <limits.h>

typedef struct {
    list_t *run_que; 
    list_t *wait_que; 
    int id; 
    int running;
} cpu_t; 

cpu_t *initialise_cpu(list_t *run, list_t *wait, int id, int time);
int calculate_time(list_t *run, list_t *wait) ; 
cpu_t *target_cpu(cpu_t *cpus[], int count); 
cpu_t *add_to_cpu(cpu_t *cpus[], cpu_t *cpu, node_t *node, int total_time); 
int cpus_empty(cpu_t *cpus[], int processors) ;  
int processes_remianing(cpu_t *cpus[], int count); 
void make_cpus(cpu_t *cpus[], int processors);
void n_p_finished(list_t *run, cpu_t *cpus[], int processors, int total_time, node_t *head, float *turn_around, float *overhead_max, float *overhead_total); 
void p_finished(list_t *run, cpu_t *cpus[], int processors, int total_time, node_t *head, float *turn_around, float *overhead_max, float *overhead_total, list_t *finished, list_t *wait); 
void get_new_process(list_t *wait, cpu_t *cpus[], cpu_t *cpu, int total_time); 
void check_progress(cpu_t *cpus[], int processors, int total_time, float *turn_around, float *overhead_max, float *overhead_total, list_t *finished);
int go_through_cpus(cpu_t *cpus[], int processors, int time, float *turn_around, float *overhead_max, float *overhead_total, list_t *finished) ;  
list_t *run_same_time(cpu_t *cpus[], int processors, list_t *to_run, int total_time); 
#endif

