#include "bin_tree_proc.h"
#include "bin_tree_err_proc.h"
#include "bin_tree_loger.h"

const char logs_dir[] = "./logs";

int main() {
    create_logs_dir(logs_dir);

    bin_tree_err_t last_err = BT_ERR_OK;

    bin_tree_t tree = {};
    bin_tree_ctor(&tree, "./logs/log.html");

    bin_tree_log_file_start(tree.log_file_ptr);

    bin_tree_create_node(&tree, NULL, false, NULL, NULL, 52);

    // DEBUG_DL_LIST_ERROR(DL_list_verify(list), "")
    // ListLogDump(&list);

    // // DL_list_insert(&list, 4, 52, &last_err);


    // DL_list_dtor(&list);

    return 0;
}