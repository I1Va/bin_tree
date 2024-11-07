#include <cstdlib>
#include <stdlib.h>

#include "bin_tree_proc.h"
#include "error_processing.h"
#include "general.h"
#include "bin_tree_err_proc.h"

#include "stack_funcs.h"
#include "stack_output.h"


bool bin_tree_ctor(bin_tree_t *tree, const char log_path[]) {
    stk_err stk_last_err = STK_ERR_OK;

    tree->n_nodes = 0;

    tree->node_stack = {};
    STACK_INIT(&tree->node_stack, 0, sizeof(bin_tree_elem_t *), tree->log_file_ptr, &stk_last_err)

    if (stk_last_err != STK_ERR_OK) {
        DEBUG_BT_LIST_ERROR(BT_ERR_STACK, "node_stack ctor failed");
        CLEAR_MEMORY(exit_mark)
    }

    strcpy(tree->log_file_path, log_path);
    tree->log_file_ptr = fopen(log_path, "a");
    if (tree->log_file_ptr == NULL) {
        DEBUG_BT_LIST_ERROR(BT_ERR_FILE_OPEN, "path: {%s}", log_path)
        CLEAR_MEMORY(exit_mark)
    }
    setbuf(tree->log_file_ptr, NULL); // disable buffering

    return true;

    exit_mark:

    stack_destroy(&tree->node_stack);
    if (tree->log_file_ptr != NULL) {
        fclose(tree->log_file_ptr);
    }
    return false;
}

bool bin_tree_dtor(bin_tree_t *tree) {
    stk_err last_stack_error = STK_ERR_OK;

    for (size_t i = 0; i < tree->node_stack.size; i++) {

        bin_tree_elem_t *node_ptr = *(bin_tree_elem_t **) stack_get_elem(&tree->node_stack, i, &last_stack_error);
        printf("deleting node : {%d}\n", node_ptr->data);

        if (last_stack_error != STK_ERR_OK) {
            debug("stack get elem by idx: [%lu] failed", i);
            return false;
        }

        FREE(node_ptr)
    }
    stack_destroy(&tree->node_stack);
    tree->root = NULL;
    tree->n_nodes = 0;

    return true;
}

bin_tree_elem_t *bin_tree_create_node(bin_tree_t *tree, bin_tree_elem_t *prev, const bool prev_left,
    bin_tree_elem_t *left, bin_tree_elem_t *right, const bin_tree_elem_value_t data)
{
    tree->n_nodes++;

    bin_tree_elem_t *node = (bin_tree_elem_t *) calloc(1, sizeof(bin_tree_elem_t));
    if (node == NULL) {
        DEBUG_BT_LIST_ERROR(BT_ERR_ALLOC, "node alloc failed");
        return NULL;
    }

    stk_err stk_last_err = STK_ERR_OK;

    stack_push(&tree->node_stack, &node, &stk_last_err);
    if (stk_last_err != STK_ERR_OK) {
        DEBUG_BT_LIST_ERROR(BT_ERR_STACK, "stack push failed");
        return NULL;
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

    return node;
}

void bin_tree_push_val(bin_tree_t *tree, bin_tree_elem_t *cur_node, bin_tree_elem_value_t val) {
    if (val < cur_node->data) {
        if (!cur_node->left) {
            bin_tree_create_node(tree, cur_node, true, NULL, NULL, val);
        } else {
            bin_tree_push_val(tree, cur_node->left, val);
        }
    } else {
        if (!cur_node->right) {
            bin_tree_create_node(tree, cur_node, false, NULL, NULL, val);
        } else {
            bin_tree_push_val(tree, cur_node->right, val);
        }
    }
}

void bin_tree_print(bin_tree_elem_t *node) {
    if (!node) {
        return;
    }
    printf("(");

    if (node->left) {
        bin_tree_print(node->left);
    }
    printf("%d", node->data);
    if (node->right) {
        bin_tree_print(node->right);
    }

    printf(")");
}

bool bin_tree_destroy(bin_tree_t *tree) {
    stk_err last_stack_error = STK_ERR_OK;

    for (size_t i = 0; i < tree->node_stack.size; i++) {
        bin_tree_elem_t *node_ptr = *(bin_tree_elem_t **) stack_pop(&tree->node_stack, &last_stack_error);

        // printf("deleting node : {%d}\n", node_ptr->data);

        if (last_stack_error != STK_ERR_OK) {
            debug("stack get elem by idx: [%lu] failed", i);
            return false;
        }

        FREE(node_ptr)
    }
    DUMP(&tree->node_stack, stdout);
    tree->root = NULL;
    tree->n_nodes = 0;
    return true;
}

void bin_tree_verify(bin_tree_elem_t tree) {

}