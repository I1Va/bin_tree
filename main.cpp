#include "bin_tree_proc.h"
#include "bin_tree_err_proc.h"
#include "bin_tree_loger.h"
#include <stdlib.h>

#include "error_processing.h"
#include "stack_funcs.h"
#include "stack_output.h"

const char logs_dir[] = "./logs";

int main() {
    create_logs_dir(logs_dir);

    bin_tree_err_t last_err = BT_ERR_OK;

    bin_tree_t tree = {};
    bin_tree_ctor(&tree, "./logs/log.html");

    bin_tree_log_file_start(tree.log_file_ptr);

    // bin_tree_elem_value_t node = {52, "fwef"};

    bin_tree_elem_t *root = bin_tree_create_node(&tree, NULL, false, NULL, NULL, {1, "root"});
    tree.root = root;

    for (int i = 0; i < 20; i++) {
        int val = rand() % 128;
        printf("added {%d}\n", val);
        bin_tree_push_val(  &tree, tree.root, {val, ""}, node_t_cmp);
    }

    bin_tree_print(tree.root, node_t_get_outp); printf("\n");

    stk_err last_stk_err = STK_ERR_OK;

    // bin_tree_elem_t *node1 = *(bin_tree_elem_t **)stack_get_elem(&tree.node_stack, 3, &last_stk_err);
    // bin_tree_elem_t *node2 = *(bin_tree_elem_t **)stack_get_elem(&tree.node_stack, 5, &last_stk_err);

    // node1->left = node2;
    // node2->right = node1;

    bin_tree_verify(tree, &last_err);

    DUMP(&tree.node_stack, stdout, tree_node_fprintf);

    TreeLogDump(&tree);


    bin_tree_dtor(&tree);
    return 0;
}