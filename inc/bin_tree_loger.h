#ifndef BIN_TREE_LOGGER_H
#define BIN_TREE_LOGGER_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdarg.h>

#include "general.h"

#include "bin_tree_proc.h"


const char LOG_IMG_DIR_NAME[] = "imgs";
const char LOG_GRAPHVIZ_CODE_DIR_NAME[] = "graphviz_code_dir";

const int EDGE_MAX_WEIGHT = 1024;
const int SIMP_EDGE_WIDTH = 5;

const size_t BORDER_SZ = 100;
const size_t LOG_WIDTH_VAL = 75;
const size_t NODE_LABEL_MAX_SZ = 128;
const size_t MAX_DIGITS_N = 32;
const size_t MAX_FMT_SZ = 32;
const size_t MAX_LOG_DIR_SZ = 128;
const size_t MAX_SHORT_IMG_PATH_SZ = 128;

const size_t MAX_IMG_DIR_SZ = sizeof(LOG_IMG_DIR_NAME) + MAX_LOG_DIR_SZ + MAX_FMT_SZ;
const size_t MAX_GRAPHVIZ_DIR_SZ = MAX_LOG_DIR_SZ + sizeof(LOG_GRAPHVIZ_CODE_DIR_NAME) + MAX_FMT_SZ;
const size_t MAX_GRAPHVIZ_FILE_SZ = MAX_GRAPHVIZ_DIR_SZ + MAX_FMT_SZ + MAX_DIGITS_N;
const size_t MAX_IMG_FILE_NAME_SZ = MAX_IMG_DIR_SZ + MAX_FMT_SZ + MAX_DIGITS_N;
const size_t MAX_SYSTEM_COMMAND_SIZE = MAX_IMG_DIR_SZ + MAX_FMT_SZ + MAX_GRAPHVIZ_FILE_SZ + MAX_IMG_FILE_NAME_SZ;

enum bin_tree_log_t {
    DL_LOG_ANALYS = 0,
    DL_LOG_DEBUG = 1,
    DL_LOG_ERROR = 2,
};

struct log_dir_t {
    char log_dir[MAX_LOG_DIR_SZ];
    char img_dir[MAX_IMG_DIR_SZ];
    char graphviz_codes_dir[MAX_GRAPHVIZ_DIR_SZ];
};

bool create_logs_dir(const char log_dir[]);
void bin_tree_log_file_start(FILE *stream);
void bin_tree_fprintf_border(FILE* stream, const char bord_char, const size_t bord_sz, bool new_line);
void bin_tree_fprintf_title(FILE *stream, const char tittle[], const char bord_char, const size_t bord_sz);
const char *bin_tree_get_log_descr(enum bin_tree_log_t log_type);
void bin_tree_log_print_time(FILE *log_output_file_ptr);
void bin_tree_print_log_func_info(FILE *log_output_file_ptr, const char file_name[], const char func_name[], const int line_idx);
void bin_tree_print_log_type(FILE *log_output_file_ptr, enum bin_tree_log_t log_type);
void bin_tree_log_var_print(FILE *log_output_file_ptr, enum bin_tree_log_t log_type, const char file_name[], const char func_name[], const int line_idx, const char fmt[], ...);
void bin_tree_log_dump(bin_tree_t *tree, const char file_name[], const char func_name[], const int line_idx);
int get_dir_files_count(const char dir_path[]);
log_dir_t bin_tree_make_graphviz_dirs(char log_file_path[]);
void graphviz_start_graph(FILE *graphviz_code_file);
void graphviz_end_graph(FILE *graphviz_code_file);
void graphviz_make_node(FILE *graphviz_code_file, bin_tree_elem_t *node,
    void (*label_func)(char *dest, const size_t max_n, const bin_tree_elem_t *node));
void graphviz_make_left_edge(FILE *graphviz_code_file, bin_tree_elem_t *node_ptr1, bin_tree_elem_t *node_ptr2, const char color[], int penwidth);
void graphviz_make_right_edge(FILE *graphviz_code_file, bin_tree_elem_t *node_ptr1, bin_tree_elem_t *node_ptr2, const char color[], int penwidth);
void graphviz_make_heavy_unvisible_edge(FILE *graphviz_code_file, bin_tree_elem_t *node_ptr1, bin_tree_elem_t *node_ptr2);
void bin_tree_log_html_insert_image(FILE *log_output_file_ptr, char short_img_path[], int width_percent);
void tree_node_fprintf(FILE *stream, void *elem_ptr);
bool bin_tree_generate_graph_img(bin_tree_t *tree, char short_img_path[]);

#define TreeLogVar(log_output_file_ptr, log_type, fmt, ...) log_var_print(log_output_file_ptr, log_type, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define TreeLogDump(tree) bin_tree_log_dump(tree, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__);

#endif // bin_tree_LOGGER_H