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

void bin_tree_rec_nodes_cnt(bin_tree_elem_t *node, size_t *nodes_cnt) {
    if (!node) {
        return;
    }
    if (*nodes_cnt > MAX_NODES_CNT) {
        return;
    }

    (*nodes_cnt)++;
    if (node->left) {
        bin_tree_rec_nodes_cnt(node->left, nodes_cnt);
    }
    if (node->right) {
        bin_tree_rec_nodes_cnt(node->right, nodes_cnt);
    }
}

void bin_tree_verify(const bin_tree_t tree, bin_tree_err_t *return_err) {
    size_t nodes_cnt = 0;
    bin_tree_rec_nodes_cnt(tree.root, &nodes_cnt);
    if (nodes_cnt > MAX_NODES_CNT) {
        bin_tree_err_add(return_err, BT_ERR_CYCLED);
        debug("tree might be cycled. nodes cnt exceeds max nodes cnt value");
        return;
    }
}
