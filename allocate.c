#include "list.h"
#include "utils.h"
#include "cpu.h"

int main(int argc, char *argv[]) {
    list_t *input; 
    int total_time = 0;  
    int processors = 0; 
    cpu_t *cpus[1024]; 
    float turn_around = 0; 
    float overhead_total = 0, overhead_max = 0; 
    int total_inputs = 0; 

    // Reading initial file from CMD
    input = read_details(argv, &processors);
 
    // Initialising CPU's
    make_cpus(cpus, processors);

    // Storing total length of the number of inputs
    total_inputs = len_oflist(input); 

    // Making sure they are sorted by arrival 
    sort_list_by_arrival(input); 

    node_t *current = input->head; 
    list_t *to_run = initialise_que(); 
    list_t *finished = initialise_que(); 

    // Iterate over the inputs
    while (current) {
        // If the arrival time is equal to current time, we add it to the list that will run all with same arrival time. 
        if (current->data->arrival_time == total_time) {
            node_t *new = make_node(current->data->arrival_time, current->data->pid, current->data->exec_time, current->data->p, current->data->exec_original); 
            to_run = insert_in_que(to_run, new);  
        } 
        // Otherwise we try to find the next one while printing everything in to_run. 
        else {
            // Running all with same arrival 
            // Making sure we sort by pid first and then execution time 
            // If execution time is same then lower pid will be on top.
            to_run = run_same_time(cpus, processors, to_run, total_time);            
            while (current->data->arrival_time != total_time) {
                // Check to see if any running proccess have completed execuuiton
                total_time++; 
                // We should go over run ques in cpus and check if we finished
                check_progress(cpus, processors, total_time, &turn_around, &overhead_max, &overhead_total, finished);
            }
            if (current->data->arrival_time == total_time) {
                node_t *new = make_node(current->data->arrival_time, current->data->pid, current->data->exec_time, current->data->p, current->data->exec_original); 
                to_run = insert_in_que(to_run, new);    
            } 
        }
        current = current->node; 
        // For last element 
        if (!current) {
            to_run = run_same_time(cpus, processors, to_run, total_time); 
            // what did we do with last element?
        }
    }
    // Go through all the elements until all cpus are empty 
    total_time = go_through_cpus(cpus, processors, total_time, &turn_around, &overhead_max, &overhead_total, finished);
    print_performance(turn_around, overhead_max, overhead_total, total_time, total_inputs);
    return 0; 
}