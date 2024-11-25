#ifndef BIN_TREE_ERR_PROC_H
#define BIN_TREE_ERR_PROC_H

#include <string.h>

enum bin_tree_err_t {
    BT_ERR_OK                       = 0ull,
    BT_ERR_FILE_OPEN                = 1ull << 0,
    BT_ERR_ALLOC                    = 1ull << 1,
    BT_ERR_SYSTEM                   = 1ull << 2,
    BT_ERR_STACK                    = 1ull << 3,
    BT_ERR_CYCLED                   = 1ull << 4,
};


const size_t ERR_DESCR_MAX_SIZE = 128;
const size_t MAX_NODES_CNT = 1ull << 17;

void bin_tree_err_get_descr(enum bin_tree_err_t err_code, char err_descr_str[]);
void bin_tree_err_add(enum bin_tree_err_t *dest, enum bin_tree_err_t add);

#ifdef _DEBUG
    #define DEBUG_BT_LIST_ERROR(err_code, str_, ...) {                                                      \
        char BT_list_err_desr_str[ERR_DESCR_MAX_SIZE];                                                    \
        bin_tree_err_get_descr(err_code, BT_list_err_desr_str); \
        printf("DESCR: '%s'\n", BT_list_err_desr_str);                                                      \
        fprintf_red(stderr, "{%s} [%s: %d]: err_descr: {%s}, message: {" str_ "}\n",              \
             __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, BT_list_err_desr_str, ##__VA_ARGS__);                    \
        fprintf(stderr, WHT); \
    }

    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define DEBUG_BT_LIST_ERROR(err_code, str_, ...) ;
#endif // _DEBUG


#endif // BIN_TREE_ERR_PROC_H