#ifndef BIN_TREE_PROC_H
#define BIN_TREE_PROC_H

#include <string.h>
#include <stdio.h>
#include "stack_funcs.h"

const size_t MAX_NODE_STRING_SZ = 128;
const size_t MAX_LOG_FILE_PATH_SZ = 128;

struct node_t {
    int value;
    char *name;
};

typedef node_t bin_tree_elem_value_t;

struct bin_tree_elem_t {
    bin_tree_elem_t *prev;
    bool left_son;

    bin_tree_elem_t *left;
    bin_tree_elem_t *right;

    bin_tree_elem_value_t data;
};

typedef bin_tree_elem_t* stack_elem_t;


struct bin_tree_t {
    bin_tree_elem_t *root;

    size_t n_nodes;


    FILE *log_file_ptr;
    char log_file_path[MAX_LOG_FILE_PATH_SZ];
    stack_t node_stack;
};


void bin_tree_push_val(bin_tree_t *tree, bin_tree_elem_t *cur_node, bin_tree_elem_value_t val,
    int (*compare_func)(const bin_tree_elem_value_t node1, const bin_tree_elem_value_t node2));
bool bin_tree_ctor(bin_tree_t *tree, const char log_path[]);
bool bin_tree_dtor(bin_tree_t *tree);
bin_tree_elem_t *bin_tree_create_node(bin_tree_t *tree, bin_tree_elem_t *prev, const bool prev_left,
    bin_tree_elem_t *left, bin_tree_elem_t *right, const bin_tree_elem_value_t data);
void bin_tree_print(bin_tree_elem_t *node, void (*outp_func)(char *dest, const size_t maxn_n, const bin_tree_elem_t *node));
bool bin_tree_clear(bin_tree_t *tree);

int node_t_cmp(const bin_tree_elem_value_t node1, const bin_tree_elem_value_t node2);
void node_t_get_label(char *dest, const size_t max_n, const bin_tree_elem_t *node);
void node_t_get_outp(char *dest, const size_t max_n, const bin_tree_elem_t *node);

#endif // BIN_TREE_PROC_H