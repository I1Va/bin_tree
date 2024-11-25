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
    DESCR_(err_code, BT_ERR_CYCLED);

    if (!error) {
        sprintf(err_descr_str, "ALL IS OK:)");
    }
    #undef DESCR_
}