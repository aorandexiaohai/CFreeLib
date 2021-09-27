#ifndef LIST_H
#define LIST_H
#include "cbase.h"
/**
 * implementation of a generic double linked list
 */
typedef struct double_list_node_inner double_list_node_t;
typedef struct double_list_inner double_list_t;

struct double_list_node_inner {
    generic_data_t data;
    double_list_node_t *prev;
    double_list_node_t *next;
};

struct double_list_inner {
    double_list_node_t *head;
    double_list_node_t *tail;
    int size;
    free_function_t free_function;
    compare_function_t compare_function;
};
double_list_t *create_double_list(free_function_t free_function);
double_list_t *create_double_list1(free_function_t free_function, compare_function_t compare_function);
double_list_node_t *push_back_double_list(double_list_t *l, generic_data_t data);
generic_data_t pop_front_double_list(double_list_t *l);
void remove_double_list(double_list_t *l, double_list_node_t *n);
void free_double_list(double_list_t *l);

/**
 * @brief try to search data in the list. Store the related result in find_node and prev_node. If data exists, prev_node
 * will be the previous node of find_node. If not, prev_node will be last accessed node.
 */
void find_double_list(double_list_t *l, generic_data_t data, double_list_node_t **find_node,
                      double_list_node_t **prev_node);
/**
 * @brief find_double_list1 is a simple version of find_double_list.
*/
double_list_node_t *find_double_list1(double_list_t *l, generic_data_t data);

/**
 * @brief If compare_function is specified, the result list will be sorted. Otherwise, the data will be inserted at the
 * back of the list.
 */
double_list_node_t *insert_double_list(double_list_t *l, generic_data_t data);

#endif