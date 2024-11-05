#include <stdio.h>

#include "bin_tree_err_proc.h"
#include "general.h"

void bin_tree_err_add(enum bin_tree_err_t *dest, enum bin_tree_err_t add) {
    *dest = (bin_tree_err_t)((unsigned long long)(*dest) | (unsigned long long) add);
}

void bin_tree_err_get_descr(const enum bin_tree_err_t err_code, char err_descr_str[]) {
    bool error = false;
    #define DESCR_(err_code, err)                 \
        {                                         \
            if (err_code & err) {                 \
                sprintf(err_descr_str, #err", "); \
                error = true;                     \
            }                                     \
        }                                         \

    DESCR_(err_code, BT_ERR_FILE_OPEN);
    DESCR_(err_code, BT_ERR_ALLOC);
    DESCR_(err_code, BT_ERR_SYSTEM);
    DESCR_(err_code, BT_ERR_STACK);

    if (!error) {
        sprintf(err_descr_str, "ALL IS OK:)");
    }
    #undef DESCR_
}

bin_tree_err_t bin_tree_verify(const bin_tree_t tree) {
    bin_tree_err_t errors = BT_ERR_OK;

    fprintf_red(stdout, "THERE IS SHOULD BE BIN_TREE_VERIFY!\n");
    if (tree.n_nodes) {};

    return errors;
}
