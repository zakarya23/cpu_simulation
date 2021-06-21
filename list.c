#include "list.h"
#include "utils.h"

data_t *pop(list_t *list) {
    assert(list); 
    node_t *head = list->head; 
    data_t *data = head->data;
    list->head = head->node; 
    free(head); 
    return data; 
}

void delete_node(list_t *list) {
    assert(list); 
    node_t *head = list->head;  
    list->head = head->node; 
    free(head);
}

int len_oflist(list_t *list) {
    assert(list); 
	node_t *temp; 
	int count=0; 
	temp = list->head;
	while(temp) {
		count++; 
		temp=temp->node; 
	}
	return count; 
}

void sort_list_by_arrival(list_t *list) {
    node_t *current, *next; 
    data_t *tmp; 
    current = list->head; 
    while (current) {
        next = current->node; 
        while (next) {
            if (current->data->arrival_time > next->data->arrival_time) {
                tmp = current->data; 
                current->data = next->data; 
                next->data = tmp; 
            }
            next = next->node; 
        }
        current = current->node; 
    }
}

void sort_list_by_pid(list_t *list) {
    node_t *current, *next; 
    data_t *tmp; 
    current = list->head; 
    while (current) {
        next = current->node; 
        while (next) {
            if (current->data->pid > next->data->pid) {
                tmp = current->data; 
                current->data = next->data; 
                next->data = tmp; 
            }
            next = next->node; 
        }
        current = current->node; 
    }
}


void sort_list_by_exec(list_t *list) {
    node_t *current, *next; 
    data_t *tmp; 
    current = list->head; 
    while (current) {
        next = current->node; 
        while (next) {
            if (current->data->exec_time > next->data->exec_time) {
                tmp = current->data; 
                current->data = next->data; 
                next->data = tmp; 
            }
            next = next->node; 
        }
        current = current->node; 
    }
}

void free_list(list_t *list) {
    assert(list); 
    node_t *current; 
    current = list->head; 
    free(current->data); 
    free(list); 
}

int compare_nodes(data_t *d1, data_t *d2) {
    int exec1 = d1->exec_time; 
    int exec2 = d2->exec_time; 
    if (exec1 < exec2) {
        return -1; 
    }
    else if (exec1 > exec2) {
        return 1; 
    }
    else {
        if (d1->pid < d2->pid) {
            return -1; 
        }
        else {
            return 1; 
        }
    }
}

node_t *get_head(list_t* list) {
    assert(list);
    assert(list->head); 
    return list->head; 
}

list_t *insert_in_que(list_t* list, node_t *node) {
    assert(list && node); 
    node->node = list->head; 
    list->head = node; 
    if (list->foot == NULL) {
        list->foot = node; 
    }
    return list; 
}

node_t *children_node(int arrival, int parent_id, int exec, int p, int original, int children_id, int k) {
    data_t *data = (data_t *)malloc(sizeof(data_t));
    assert(data);
    node_t *node = (node_t *)malloc(sizeof(node_t)); 
    assert(node); 
    data->arrival_time = arrival; 
    data->pid = parent_id; 
    double new_time = (((exec * 1.0) / k ) + 1); 
    int time = round_off(&new_time);
    // printf("%f \n", new_time);
    data->exec_time = time; 
    data->p = p; 
    data->exec_original = original; 
    node->data = data; 
    node->data->split_id = children_id; 
    node->node = NULL; 
    return node; 

}

node_t *make_node(int arrival, int id, int exec, int p, int original) {
    data_t *data = (data_t *)malloc(sizeof(data_t));
    assert(data);
    node_t *node = (node_t *)malloc(sizeof(node_t)); 
    assert(node); 
    data->arrival_time = arrival; 
    data->pid = id; 
    data->exec_time = exec; 
    data->p = p; 
    data->exec_original = original; 
    node->data = data; 
    node->node = NULL; 
    return node; 
}

void free_que(list_t *list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->node;
		free(prev);
	}
	free(list);
}

list_t *initialise_que(void) {
    list_t *list = (list_t*)malloc(sizeof(list_t)); 
    assert(list); 
    list->head = NULL; 
    list->foot = NULL; 
    return list; 
}

list_t *read_input_file(FILE *fptr) {
    list_t *input;
    char buffer[100]; 
    input = initialise_que(); 
    while (fgets(buffer, 512, fptr))     {
    // Separating words by space
    char *words = strtok(buffer, " "); 
    int count = 0, arrival , pid, execution, par; 
    while (words) {
        if (count == 0) {
            arrival = atoi(words); 
        }
        if (count == 1) {
            pid = atoi(words); 
        }
        if (count == 2) {
            execution = atoi(words); 
        }
        if (count == 3) {
            if (words[0] == 'n') {
                par = 0; 
            }
            else {
                par = 1; 
            }
        }
        count++;
        words = strtok(NULL, " "); 
    }
    node_t *newnode = make_node(arrival, pid, execution, par, execution); 
    input = insert_in_que(input, newnode); 
    }
    return input; 
}