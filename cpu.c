#include "list.h"
#include "cpu.h"
#include "utils.h"

list_t *run_same_time(cpu_t *cpus[], int processors, list_t *to_run, int total_time) {
    sort_list_by_pid(to_run); 
    sort_list_by_exec(to_run);
    node_t *head = to_run->head; 
    while (head) {
        if (!head->data->p) {  
            node_t *new = make_node(head->data->arrival_time, head->data->pid, head->data->exec_time, head->data->p, head->data->exec_original); 
            cpu_t *cpu_target = target_cpu(cpus, processors); 
            cpu_target = add_to_cpu(cpus, cpu_target, new, total_time); 
        }
        else {
            int id = 0 ; 
            for (int i = 0; i < processors ; i++) {
                node_t *n1 = children_node(head->data->arrival_time, head->data->pid, head->data->exec_time, head->data->p, head->data->exec_original, id, processors);
                cpu_t *cpu_target = target_cpu(cpus, processors); 
                cpu_target = add_to_cpu(cpus, cpu_target, n1, total_time); 
                id++;
            }
        }
        head = head->node; 
    }
    to_run = initialise_que(); 
    return to_run; 
}
int go_through_cpus(cpu_t *cpus[], int processors, int time, float *turn_around, float *overhead_max, float *overhead_total, list_t *finished) {
    // printf("%f\n", turn_around);
    int total_time = time; 
    while (cpus_empty(cpus, processors) != 0) {
        total_time++; 
        for (int i = 0; i < processors ; i++) {
            list_t *run = cpus[i]->run_que; 
            list_t *wait = cpus[i]->wait_que; 
            sort_list_by_arrival(wait); 

            if (len_oflist(run) > 0) {
                node_t *head = get_head(run); 
                get_head(run)->data->exec_time = get_head(run)->data->exec_time - 1; 
                if (get_head(run)->data->exec_time == 0) {
                    if (!head->data->p) {
                        n_p_finished(run, cpus, processors, total_time, head, &(*turn_around), &(*overhead_max), &(*overhead_total));
                    }
                    else {
                        p_finished(run, cpus, processors, total_time, head, &(*turn_around), &(*overhead_max), &(*overhead_total), finished, wait);
                    }
                    if (len_oflist(wait) > 0) {
                        get_new_process(wait, cpus, cpus[i], total_time);
                    }
                }
            }
            else if (len_oflist(wait) > 0) {
                get_new_process(wait, cpus, cpus[i], total_time);
            }
        }
    }
    return total_time; 
}

void check_progress(cpu_t *cpus[], int processors, int total_time, float *turn_around, float *overhead_max, float *overhead_total, list_t *finished) {
    for (int i = 0; i < processors ; i++) {
        list_t *run = cpus[i]->run_que;  
        list_t *wait = cpus[i]->wait_que; 
        // GETTING THE NODE CURRENTLY RUNNING AND CHECKING IF ITS DONE! 
        if (len_oflist(run) > 0) {
            node_t *head = get_head(run); 
            // REDUCING TIME 
            get_head(run)->data->exec_time = get_head(run)->data->exec_time - 1; 
            // IF ITS DONE 
            if (get_head(run)->data->exec_time == 0) {
                if (!head->data->p) {
                    n_p_finished(run, cpus, processors, total_time, head, &(*turn_around), &(*overhead_max), &(*overhead_total));
                }
                else {
                    p_finished(run, cpus, processors, total_time, head, &(*turn_around), &(*overhead_max), &(*overhead_total), finished, wait);
                }
            }
        }
    }
}

void get_new_process(list_t *wait, cpu_t *cpus[], cpu_t *cpu, int total_time) {
    sort_list_by_arrival(wait);
    data_t *n = pop(wait);
    node_t *new = make_node(n->arrival_time, n->pid, n->exec_time, n->p, n->exec_original); 
    cpu = add_to_cpu(cpus, cpu, new, total_time);
}

void p_finished(list_t *run, cpu_t *cpus[], int processors, int total_time, node_t *head, float *turn_around, float *overhead_max, float *overhead_total, list_t *finished, list_t *wait) {
    data_t *data = pop(run);
    node_t *new = make_node(data->arrival_time, data->pid, data->exec_time, data->p, data->exec_original);
    int found = 0; 
    for (int i = 0; i < processors ; i++) {
        list_t *r = cpus[i]->run_que;  
        list_t *w = cpus[i]->wait_que; 
        
        node_t *tmp = r->head;
        while (tmp) {
            if (tmp->data->pid == new->data->pid) {
                found++; 
            }
            tmp = tmp->node; 
        } 
        tmp = w->head; 
        while (tmp) {
            if (tmp->data->pid == new->data->pid) {
                found++; 
            }
            tmp = tmp->node; 
        } 
    }
    if (found == 0) {
        finished = insert_in_que(finished, new); 
        node_t *ptr = finished->head; 
        int left = processes_remianing(cpus, processors); 
        printf(FINISHED, total_time, ptr->data->pid, left); 
        *turn_around += total_time - ptr->data->arrival_time;
        float for_one = ((total_time - head->data->arrival_time) * 1.0) / head->data->exec_original;  
        if (for_one > *overhead_max) {
            *overhead_max = for_one; 
        }
        *overhead_total += for_one; 
        for (int i = 0 ; i< processors; i++) {
            list_t *w = cpus[i]->wait_que;
            if (len_oflist(w) > 0) {
                sort_list_by_arrival(wait);
                data_t *n = pop(w);
                node_t *new = make_node(n->arrival_time, n->pid, n->exec_time, n->p, n->exec_original); 
                cpus[i] = add_to_cpu(cpus, cpus[i], new, total_time); 
            }
        }
        finished = initialise_que(); 
    }
    else {
        finished = insert_in_que(finished, new); 
    }
}

void n_p_finished(list_t *run, cpu_t *cpus[], int processors, int total_time, node_t *head, float *turn_around, float *overhead_max, float *overhead_total) {
    data_t *data = pop(run); 
    make_node(data->arrival_time, data->pid, data->exec_time, data->p, data->exec_original);
    int left = processes_remianing(cpus, processors); 
    printf(FINISHED, total_time, head->data->pid, left); 
    *turn_around += total_time - head->data->arrival_time;
    float for_one = ((total_time - head->data->arrival_time) * 1.0) / head->data->exec_original;  
    if (for_one > *overhead_max) {
        *overhead_max = for_one; 
    }
    *overhead_total += for_one; 
}

void make_cpus(cpu_t *cpus[], int processors) {
    for (int i = 0; i < processors ; i++) {
        list_t *wait, *run; 
        run = initialise_que(); 
        wait = initialise_que(); 
        cpus[i] = initialise_cpu(run, wait, i, 0); 
    }
}

int processes_remianing(cpu_t *cpus[], int count) {
    double total = 0; 
    for (int i = 0; i< count; i++) {
        cpu_t *cpu = cpus[i]; 
        list_t *run = cpu->run_que; 
        node_t *head = run->head; 
        list_t *wait = cpu->wait_que; 

        while (head) {
            if (!head->data->p) {
                total += 1;
            }
            else {
                total += 0.5; 
            }
            head = head->node; 
        }
        head = wait->head; 
        while (head) {
            if (!head->data->p) {
                total += 1;
            }
            else {
                total += 0.5; 
            }
            head = head->node; 
        }   
    }
    return total; 
}

cpu_t *initialise_cpu(list_t *run, list_t *wait, int id, int running) {
    cpu_t *cpu = malloc(sizeof(cpu)); 
    assert(cpu); 
    assert(run && wait); 
    cpu->run_que = run; 
    cpu->wait_que = wait; 
    cpu->id = id; 
    cpu->running = running; 
    return cpu; 
}

cpu_t *target_cpu(cpu_t *cpus[], int count) {
    cpu_t *target; 
    int time = INT_MAX; 
    // FOR EACH CPU 
    // CHECK BOTH QUES AND ALL NODES FOR EACH QUE 
    for (int i = 0;  i < count; i++) {
        int total_time = 0; 
        cpu_t *cpu = cpus[i]; 
        list_t *run = cpu->run_que; 
        list_t *wait = cpu->wait_que; 
        total_time += calculate_time(run, wait); 
        if (total_time == time && cpu->id < target->id) {
             target = cpu;       
        }
        else if (total_time < time) {
            time = total_time;                 
            target = cpu; 
        }
    }
    return target; 
}

int calculate_time(list_t *run, list_t *wait) {
    node_t *head = run->head; 
    int time = 0; 
    while (head) {
        time += head->data->exec_time; 
        head = head->node; 
    }

    head = wait->head; 
    while (head) {
        time += head->data->exec_time; 
        head = head->node; 
    }
    return time; 
}

int cpus_empty(cpu_t *cpus[], int processors) {
    int empty = 0; 
    for (int i = 0; i < processors; i++) {
        list_t *run = cpus[i]->run_que; 
        list_t *wait = cpus[i]->wait_que;
        if (len_oflist(run) > 0 || len_oflist(wait) > 0) {
            empty++; 
        }
    }
    return empty;
}

// ALONG THE WAY KEEP TRACK OF TIME SO WE KNOW WHEN WE GINISHED A PROCVESS
cpu_t *add_to_cpu(cpu_t *cpus[], cpu_t *cpu, node_t *node, int total_time) {
    assert(cpu && node); 
    list_t *run = cpu->run_que; 
    list_t *wait = cpu->wait_que; 
    if (len_oflist(run) == 0) {
        if (len_oflist(wait) > 0) {
            sort_list_by_pid(wait); 
            sort_list_by_arrival(wait); 
            if ((compare_nodes(get_head(wait)->data, node->data) < 0 )) {
                node_t *old = get_head(wait); 
                node_t *new_node = make_node(old->data->arrival_time, old->data->pid, old->data->exec_time, old->data->p, old->data->exec_original);
                delete_node(wait); 
                run = insert_in_que(run, new_node); 
                wait = insert_in_que(wait, node); 
                if (!new_node->data->p){
                    printf(RUNNING,  total_time, new_node->data->pid, new_node->data->exec_time, cpu->id); 
                }
                else {
                    printf(RUNNING_PAR,  total_time, new_node->data->pid, new_node->data->split_id, new_node->data->exec_time, cpu->id); 
                }
            }            
            else {
                run = insert_in_que(run, node); 
                if (!node->data->p){
                    printf(RUNNING,  total_time, node->data->pid, node->data->exec_time, cpu->id); 
                }
                else {
                    printf(RUNNING_PAR,  total_time, node->data->pid, node->data->split_id, node->data->exec_time, cpu->id); 
                }
            }
        }
        else {
            run = insert_in_que(run, node); 
            if (!node->data->p){
                printf(RUNNING,  total_time, node->data->pid, node->data->exec_time, cpu->id); 
            }
            else {
                printf(RUNNING_PAR,  total_time, node->data->pid, node->data->split_id, node->data->exec_time, cpu->id); 
            }
        }
    }
    else if ((compare_nodes(get_head(run)->data, node->data) < 0 )) {
        wait = insert_in_que(wait, node); 
    }
    else {
        node_t *old = get_head(run); 
        node_t *old_node = make_node(old->data->arrival_time, old->data->pid, old->data->exec_time, old->data->p, old->data->exec_original);
        delete_node(run); 
        run = insert_in_que(run, node);  
        if (!node->data->p) {
            printf(RUNNING,  total_time, node->data->pid, node->data->exec_time, cpu->id); 
        }
        else {
            printf(RUNNING_PAR,  total_time, node->data->pid, node->data->split_id, node->data->exec_time, cpu->id); 
        }

        wait = insert_in_que(wait, old_node); 
    }
    cpu->running = len_oflist(run) + len_oflist(wait); 
    return cpu; 
}