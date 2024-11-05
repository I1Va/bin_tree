#include <stdlib.h>

#include "bin_tree_proc.h"
#include "error_processing.h"
#include "general.h"
#include "bin_tree_err_proc.h"


bool bin_tree_ctor(bin_tree_t *tree, const char log_path[]) {
    stk_err stk_last_err = STK_ERR_OK;

    tree->n_nodes = 0;

    tree->node_stack = {};
    STACK_INIT(&tree->node_stack, 0, &stk_last_err)
    if (stk_last_err != STK_ERR_OK) {
        DEBUG_BT_LIST_ERROR(BT_ERR_STACK, "node_stack ctor failed");
        CLEAR_MEMORY(exit_mark)
    }

    ON_DEBUG
    (
        strcpy(tree->log_file_path, log_path);
        tree->log_file_ptr = fopen(log_path, "a");
        if (tree->log_file_ptr == NULL) {
            DEBUG_BT_LIST_ERROR(BT_ERR_FILE_OPEN, "path: {%s}", log_path)
            CLEAR_MEMORY(exit_mark)
        }
        setbuf(tree->log_file_ptr, NULL); // disable buffering
    )

    return true;

    exit_mark:

    stack_destroy(&tree->node_stack);
    if (tree->log_file_ptr != NULL) {
        fclose(tree->log_file_ptr);
    }
    return false;
}

bool bin_tree_create_node(bin_tree_t *tree, bin_tree_elem_t *prev, const bool prev_left,
    bin_tree_elem_t *left, bin_tree_elem_t *right, const bin_tree_elem_value_t data)
{
    tree->n_nodes++;

    bin_tree_elem_t *node = (bin_tree_elem_t *) calloc(1, sizeof(bin_tree_elem_t));
    if (node == NULL) {
        DEBUG_BT_LIST_ERROR(BT_ERR_ALLOC, "node alloc failed");
        return false;
    }

    stk_err stk_last_err = STK_ERR_OK;

    stack_push(&tree->node_stack, node, &stk_last_err);
    if (stk_last_err != STK_ERR_OK) {
        DEBUG_BT_LIST_ERROR(BT_ERR_STACK, "stack push failed");
        return false;
    }

    if (prev) {
        if (prev_left) {
        prev->left = node;
        } else {
            prev->right = node;
        }
        node->prev = prev;
    }

    node->left = left;
    node->right = right;
    node->data = data;

    return true;
}
