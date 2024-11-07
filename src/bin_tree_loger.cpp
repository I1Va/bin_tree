#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <dirent.h>

#include <stdarg.h>

#include "bin_tree_proc.h"
#include "error_processing.h"
#include "general.h"
#include "bin_tree_loger.h"
#include "bin_tree_err_proc.h"
#include "stack_funcs.h"


// ON_DEBUG
// (


void bin_tree_log_file_start(FILE *stream) {
    fprintf(stream, "<pre>\n");
}

void bin_tree_fprintf_border(FILE* stream, const char bord_char, const size_t bord_sz, bool new_line) {
    fprintf(stream, HTML_BLACK);
    for (size_t i = 0; i < bord_sz; i++) {
        fputc(bord_char, stream);
    }
    if (new_line) {
        fputc('\n', stream);
    }
}


void bin_tree_fprintf_title(FILE *stream, const char tittle[], const char bord_char, const size_t bord_sz) {
    assert(tittle != NULL);
    size_t tittle_sz = strlen(tittle);
    if (bord_sz < tittle_sz) {
        return;
    }
    size_t len = bord_sz - tittle_sz;
    bin_tree_fprintf_border(stream, bord_char, len / 2, false);
    fprintf_html_red(stream, "%s", tittle);
    bin_tree_fprintf_border(stream, bord_char, (len + 1) / 2, true);
}

const char *bin_tree_get_log_descr(enum bin_tree_log_t log_type) {
    // #define DL_DESCR_(log_type) case log_type: return #log_type;

    switch (log_type) {
        case DL_LOG_ANALYS: return "DL_LOG_ANALYS";
        case DL_LOG_DEBUG: return "DL_LOG_DEBUG";
        case DL_LOG_ERROR: return "DL_LOG_ERROR";
        default: return "STRANGE LOG_TYPE";
    }
    // #undef DL_DESCR_
}

void bin_tree_log_print_time(FILE *log_output_file_ptr) {
    time_t cur_time;
    time(&cur_time);
    struct tm *now = localtime(&cur_time);

    char date_str[date_nmemb] = {};
    char time_str[time_nmemb] = {};
    strftime(date_str, date_nmemb, "%m/%d/%Y", now);
    strftime(time_str, time_nmemb, "%T", now);

    double milliseconds = ((double) clock()) / CLOCKS_PER_SEC;
    fprintf(log_output_file_ptr, "Date: %s Time: %s Ms: {%f}\n", date_str, time_str, milliseconds);
}

void bin_tree_print_log_func_info(FILE *log_output_file_ptr, const char file_name[], const char func_name[], const int line_idx) {
    fprintf(log_output_file_ptr, "file : {%s}; func: {%s}; line: {%d}\n", file_name, func_name, line_idx);
}

void bin_tree_print_log_type(FILE *log_output_file_ptr, enum bin_tree_log_t log_type) {
    fprintf(log_output_file_ptr, "log_type: {%s}\n", bin_tree_get_log_descr(log_type));
}

void bin_tree_log_var_print(FILE *log_output_file_ptr, enum bin_tree_log_t log_type, const char file_name[], const char func_name[], const int line_idx, const char fmt[], ...) {
    bin_tree_fprintf_title(log_output_file_ptr, "LOG_VAR", '-', BORDER_SZ);
    bin_tree_print_log_type(log_output_file_ptr, log_type);
    bin_tree_log_print_time(log_output_file_ptr);
    bin_tree_print_log_func_info(log_output_file_ptr, file_name, func_name, line_idx);

    if (line_idx) {}

    va_list args;
    va_start(args, fmt);
    vfprintf(log_output_file_ptr, fmt, args);

    va_end(args);
    fprintf(log_output_file_ptr, "\n");

    bin_tree_fprintf_border(log_output_file_ptr, '-', BORDER_SZ, true);
}

int get_dir_files_count(const char dir_path[]) {
    int file_count = 0;

    DIR *dirp;
    struct dirent *entry;

    dirp = opendir(dir_path);
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) {
            file_count++;
        }
    }
    closedir(dirp);
    return file_count;
}

log_dir_t bin_tree_make_graphviz_dirs(char log_file_path[]) {
    log_dir_t logs_dir_obj = {};

    char *log_file_path_ptr = (char *) log_file_path;
    char *log_dir_ptr = strrchr(log_file_path_ptr, '/');
    memcpy(logs_dir_obj.log_dir, log_file_path, (size_t) (log_dir_ptr - log_file_path) * sizeof(char));




    snprintf(logs_dir_obj.img_dir, MAX_LOG_FILE_PATH_SZ, "%s/%s", logs_dir_obj.log_dir, LOG_IMG_DIR_NAME);
    snprintf(logs_dir_obj.graphviz_codes_dir, MAX_LOG_FILE_PATH_SZ, "%s/%s", logs_dir_obj.log_dir, LOG_GRAPHVIZ_CODE_DIR_NAME);

    char mkdir_img_command[MAX_SYSTEM_COMMAND_SIZE] = {};
    snprintf(mkdir_img_command, MAX_SYSTEM_COMMAND_SIZE, "mkdir -p %s", logs_dir_obj.img_dir);
    if (system(mkdir_img_command) != 0) {
        DEBUG_BT_LIST_ERROR(BT_ERR_SYSTEM, "execution: '%s' failed", mkdir_img_command);
    }

    char mkdir_graphviz_code_command[MAX_SYSTEM_COMMAND_SIZE] = {};
    snprintf(mkdir_graphviz_code_command, MAX_SYSTEM_COMMAND_SIZE, "mkdir -p %s", logs_dir_obj.graphviz_codes_dir);
    if (system(mkdir_graphviz_code_command) != 0) {
        DEBUG_BT_LIST_ERROR(BT_ERR_SYSTEM, "execution: '%s' failed", mkdir_graphviz_code_command);
    }


    // printf("count: %d\n", get_dir_files_count(logs_dir_obj.img_dir));
    // printf("%s\n", logs_dir_obj.log_dir);
    // printf("%s\n", logs_dir_obj.img_dir);
    // printf("%s\n", logs_dir_obj.graphviz_codes_dir);
    return logs_dir_obj;
}

void graphviz_start_graph(FILE *graphviz_code_file) {
    fprintf(graphviz_code_file, "digraph G{\n");
    fprintf(graphviz_code_file, "   rankdir=TB;\n");
}

void graphviz_end_graph(FILE *graphviz_code_file) {
    fprintf(graphviz_code_file, "}");
    fclose(graphviz_code_file);
}

void graphviz_make_node(FILE *graphviz_code_file, bin_tree_elem_t *node) { // FIXME: function depends on bin_tree_elem_value_t
    fprintf(graphviz_code_file, "   NODE%p[pin=true,shape=\"Mrecord\",label=\"{data: %d | {<left> %p | <right> %p}}\"];\n", node, node->data, node->left, node->right);
} //

void graphviz_make_heavy_unvisible_edge(FILE *graphviz_code_file, bin_tree_elem_t *node_ptr1, bin_tree_elem_t *node_ptr2) {
    fprintf(graphviz_code_file, "   NODE%p -> NODE%p [weight=%d,color=\"white\"];\n", node_ptr1, node_ptr2, EDGE_MAX_WEIGHT);
    // FIXME: кажется, что делать рербра белыми - костыль
}

void graphviz_make_left_edge(FILE *graphviz_code_file, bin_tree_elem_t *node_ptr1, bin_tree_elem_t *node_ptr2, const char color[] = "black", int penwidth=SIMP_EDGE_WIDTH) {
    fprintf(graphviz_code_file, "   NODE%p:left -> NODE%p [color=\"%s\",penwidth=%d];\n", node_ptr1, node_ptr2, color, penwidth);
    // FIXME: кажется, что делать рербра белыми - костыль
}

void graphviz_make_right_edge(FILE *graphviz_code_file, bin_tree_elem_t *node_ptr1, bin_tree_elem_t *node_ptr2, const char color[] = "black", int penwidth=SIMP_EDGE_WIDTH) {
    fprintf(graphviz_code_file, "   NODE%p:right -> NODE%p [color=\"%s\",penwidth=%d];\n", node_ptr1, node_ptr2, color, penwidth);
    // FIXME: кажется, что делать рербра белыми - костыль
}

void bin_tree_log_html_insert_image(FILE *log_output_file_ptr, char short_img_path[], int width_percent) {
    // img_path = strrchr(img_path, '/');
    // img_path = strrchr(img_path, '/');
    fprintf(log_output_file_ptr, "<img src=\"%s\" width=\"%d%%\">\n", short_img_path, width_percent);
}

void tree_node_fprintf(FILE *stream, void *elem_ptr) {
    fprintf(stream, "{%d}", ((bin_tree_elem_t *) elem_ptr)->data);
}

bool bin_tree_generate_graph_img(bin_tree_t *tree, char short_img_path[]) {
    log_dir_t log_dir_obj = bin_tree_make_graphviz_dirs(tree->log_file_path);

    int graph_num = get_dir_files_count(log_dir_obj.graphviz_codes_dir);

    char graphviz_code_file_name[MAX_LOG_FILE_PATH_SZ] = {};
    snprintf(graphviz_code_file_name, MAX_LOG_FILE_PATH_SZ, "%s/%d.dot", log_dir_obj.graphviz_codes_dir, graph_num);
    // printf("gr file: %s\n", graphviz_code_file_name);
    char img_file_name[MAX_LOG_FILE_PATH_SZ] = {};

    snprintf(img_file_name, MAX_LOG_FILE_PATH_SZ, "%s/%d.png", log_dir_obj.img_dir, graph_num);
    snprintf(short_img_path, MAX_LOG_FILE_PATH_SZ, "%s/%d.png", LOG_IMG_DIR_NAME, graph_num);
    // printf("img file: %s\n", graphviz_code_file_name);

    FILE* graphviz_code_file = fopen(graphviz_code_file_name, "w");
    if (graphviz_code_file == NULL) {
        return false;
    }
    // MAKING GRAPH

    graphviz_start_graph(graphviz_code_file);

    for (size_t i = 0; i < tree->node_stack.size; i++) {
        stk_err stk_last_err = STK_ERR_OK;
        bin_tree_elem_t *node = *(bin_tree_elem_t **) stack_get_elem(&tree->node_stack, i, &stk_last_err);
        if (stk_last_err != STK_ERR_OK) {
            debug("stack_get_elem by idx [%lu] failed", i);
            return false;
        }

        graphviz_make_node(graphviz_code_file, node);

    }

    for (size_t i = 0; i < tree->node_stack.size; i++) {
        stk_err stk_last_err = STK_ERR_OK;
        bin_tree_elem_t *node = *(bin_tree_elem_t **) stack_get_elem(&tree->node_stack, i, &stk_last_err);
        if (stk_last_err != STK_ERR_OK) {
            debug("stack_get_elem by idx [%lu] failed", i);
            return false;
        }
        if (node->left) {
            graphviz_make_left_edge(graphviz_code_file, node, node->left, "green", 2);
        }
        if (node->right) {
            graphviz_make_right_edge(graphviz_code_file, node, node->right, "blue", 2);
        }

    }

    // for (int i = 0; i < tree->size; i++) {
    //     graphviz_make_node(graphviz_code_file, tree->data[i]);
    // }
    // for (int i = 1; i < tree->size; i++) {
    //     graphviz_make_heavy_unvisible_edge(graphviz_code_file, i - 1, i);
    // }

    // // tree->data[0].next = 0;
    // // tree->data[0].next = 0;
    // // tree->data[2].prev = 4;
    // // tree->data[3].next = 3;
    // // tree->data[1].prev = 5; // EXAMPLE
    // // tree->data[4].next = 1;
    // // tree->data[3].next = 2;
    // // tree->data[8].next = 6;
    // // tree->data[7].prev = 4;
    // // tree->data[5].next = 7;

    // for (int i = 0; i < tree->size; i++) {
    //     if (tree->data[i].next != -1) {
    //         graphviz_make_edge(graphviz_code_file, i, tree->data[i].next, "green", 2);
    //     }
    //     if (tree->data[i].prev != -1) {
    //         graphviz_make_edge(graphviz_code_file, tree->data[i].prev, i, "blue", 1);
    //     }
    // }

    graphviz_end_graph(graphviz_code_file);
    // MAKING GRAPH

    char draw_graph_command[MAX_SYSTEM_COMMAND_SIZE] = {};
    snprintf(draw_graph_command, MAX_SYSTEM_COMMAND_SIZE, "dot %s -Tpng -o %s",
        graphviz_code_file_name, img_file_name);
    if (system(draw_graph_command) != 0) {
        DEBUG_BT_LIST_ERROR(BT_ERR_SYSTEM, "execution: '%s' failed", draw_graph_command);
        return false;
    }
    return true;
}

bool create_logs_dir(const char log_dir[]) {
    char mkdir_command[MAX_SYSTEM_COMMAND_SIZE] = {};
    snprintf(mkdir_command, MAX_SYSTEM_COMMAND_SIZE, "mkdir -p %s", log_dir);
    if (system(mkdir_command) != 0) {
        DEBUG_BT_LIST_ERROR(BT_ERR_SYSTEM, "execution: '%s' failed", mkdir_command);
        return false;
    }
    return true;
}

void bin_tree_log_dump(bin_tree_t *tree, const char file_name[], const char func_name[], const int line_idx) {
    if (tree == NULL) {
        return;
    }

    if (tree->log_file_ptr == NULL) {
        return;
    }

    bin_tree_fprintf_title(tree->log_file_ptr, "DL_tree DUMP", '-', BORDER_SZ);
    bin_tree_print_log_type(tree->log_file_ptr, DL_LOG_DEBUG);
    bin_tree_log_print_time(tree->log_file_ptr);
    bin_tree_print_log_func_info(tree->log_file_ptr, file_name, func_name, line_idx);

    fprintf_html_red(tree->log_file_ptr, "tree [%p] at %s:%d\n", tree, file_name, line_idx);
    fprintf_html_grn(tree->log_file_ptr, "size: [%5lu]\n", tree->n_nodes);

    char short_img_path[MAX_LOG_FILE_PATH_SZ] = {};
    bin_tree_generate_graph_img(tree, short_img_path);
    bin_tree_log_html_insert_image(tree->log_file_ptr, short_img_path, LOG_WIDTH_VAL);
    // for (int i = 0; i < tree.size; i++) {
    //     fprintf(tree.log_output_file_ptr, tree.data)
    // }

    bin_tree_fprintf_border(tree->log_file_ptr, '-', BORDER_SZ, true);
}

// )
