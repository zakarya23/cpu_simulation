#include "utils.h"

void print_performance(float turn_around, float max, float total, int total_time, int num_input) {
    double final = turn_around/num_input; 
    printf(TURNAROUND, round_off(&final));
    printf(TIMEOVERHEAD, max, total / (num_input));
    printf(MAKESPAN, total_time);
}

int round_off(double *number) {
    int new = (int) (*number + 0.9); 
    return new; 
}

list_t *read_details(char *argv[], int *processors) {
    FILE *fptr; 
    list_t *input; 
    for (int i = 0; i <=4 ; i++) {
        if (!strcmp(argv[i], "-p")) {
            *processors = atoi(argv[i+1]);
        }
        else if (!strcmp(argv[i], "-f")) {
            fptr = fopen(argv[i+1], "r"); 
            assert(fptr); 
        }
    }
    input = read_input_file(fptr);
    return input; 
}