#ifndef BIN_TREE_PROC_H
#define BIN_TREE_PROC_H

#include <string.h>
#include <stdio.h>



typedef int bin_tree_elem_value_t;

const char LOG_IMG_DIR_NAME[] = "imgs";
const char LOG_GRAPHVIZ_CODE_DIR_NAME[] = "graphviz_code_dir";
const size_t MAX_DIGITS_N = 32;
const size_t MAX_LOG_FILE_PATH_SZ = 128;
const size_t MAX_SYSTEM_COMMAND_SIZE = 128;



struct bin_tree_elem_t {
    bin_tree_elem_t *prev;
    bin_tree_elem_t *left;
    bin_tree_elem_t *right;

    bin_tree_elem_value_t data;
};

typedef bin_tree_elem_t* stack_elem_t;
#include "stack_funcs.h"

struct bin_tree_t {
    bin_tree_elem_t *root;

    size_t n_nodes;


    FILE *log_file_ptr;
    char log_file_path[MAX_LOG_FILE_PATH_SZ];
    stack_t node_stack;
};


void bin_tree_push_val(bin_tree_t *tree, bin_tree_elem_t *cur_node, bin_tree_elem_value_t val);
bool bin_tree_ctor(bin_tree_t *tree, const char log_path[]);
bool bin_tree_dtor(bin_tree_t *tree);
bin_tree_elem_t *bin_tree_create_node(bin_tree_t *tree, bin_tree_elem_t *prev, const bool prev_left,
    bin_tree_elem_t *left, bin_tree_elem_t *right, const bin_tree_elem_value_t data);
void bin_tree_print(bin_tree_elem_t *node);
bool bin_tree_destroy(bin_tree_t *tree);

const bin_tree_elem_value_t BIN_TREE_POISON_VALUE = 0xBADBAD;







#endif // BIN_TREE_PROC_H