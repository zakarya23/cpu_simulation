#ifndef LISTH 
#define LISTH
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct node node_t; 

typedef struct {
    int arrival_time; 
    int pid; 
    int exec_time; 
    int p; 
    int exec_original; 
    int split_id; 
    int done; 
} data_t; 

struct node{
    data_t *data; 
    node_t *node; 
}; 

typedef struct {
    node_t *head; 
    node_t *foot; 
} list_t; 

list_t *initialise_que(void); 
node_t *make_node(int arrival, int id, int exec, int p, int original);
list_t* insert_in_que(list_t* list, node_t *node); 
int compare_nodes(data_t *d1, data_t *d2); 
node_t *get_head(list_t* list); 
void free_list(list_t *list); 
void sort_list_by_arrival(list_t *list);
int len_oflist(list_t *list);
data_t *pop(list_t *list); 
void sort_list_by_pid(list_t *list) ;
void free_que(list_t *list) ; 
void sort_list_by_exec(list_t *list) ;
list_t *read_input_file(FILE *fptr); 
void delete_node(list_t *list); 
node_t *children_node(int arrival, int parent_id, int exec, int p, int original, int children_id, int k) ; 
#endif 