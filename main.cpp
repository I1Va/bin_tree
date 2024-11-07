#include "bin_tree_proc.h"
#include "bin_tree_err_proc.h"
#include "bin_tree_loger.h"
#include <cstdlib>

const char logs_dir[] = "./logs";

int main() {
    create_logs_dir(logs_dir);

    bin_tree_err_t last_err = BT_ERR_OK;

    bin_tree_t tree = {};
    bin_tree_ctor(&tree, "./logs/log.html");

    bin_tree_log_file_start(tree.log_file_ptr);

    bin_tree_elem_t *root = bin_tree_create_node(&tree, NULL, false, NULL, NULL, 52);
    tree.root = root;

    for (int i = 0; i < 20; i++) {
        bin_tree_push_val(&tree, tree.root, rand() % 128);
    }

    bin_tree_print(tree.root); printf("\n");

    bin_tree_destroy(&tree);
    // DEBUG_DL_LIST_ERROR(DL_list_verify(list), "")
    TreeLogDump(&tree);

    // // DL_list_insert(&list, 4, 52, &last_err);


    bin_tree_dtor(&tree);
    return 0;
}