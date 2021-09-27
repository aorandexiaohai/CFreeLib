#ifndef LIST_H
#define LIST_H
#include "cbase.h"
/**
 * implementation of a generic single linked list
 */
typedef struct single_list_node_inner single_list_node_t;
typedef struct single_list_inner single_list_t;

struct single_list_node_inner {
    generic_data_t data;
    single_list_node_t *next;
};

struct single_list_inner {
    single_list_node_t *head;
    single_list_node_t *tail;
    int size;
    free_function_t free_function;
    compare_function_t compare_function;
};
single_list_t *create_single_list(free_function_t free_function);
single_list_t *create_single_list1(free_function_t free_function, compare_function_t compare_function);
/**
 * @brief If compare_function is specified, the result list will be sorted. Otherwise, the data will be inserted at the
 * back of the list.
 */
single_list_node_t *insert_single_list(single_list_t *l, generic_data_t data);
single_list_node_t *push_back_single_list(single_list_t *l, generic_data_t data);
generic_data_t pop_front_single_list(single_list_t *l);
void remove_single_list(single_list_t *l, single_list_node_t *n);
void free_single_list(single_list_t *l);
/**
 * @brief try to search data in the list. Store the related result in find_node and prev_node. If data exists, prev_node
 * will be the previous node of find_node. If not, prev_node will be last accessed node.
 */
void find_single_list(single_list_t *l, generic_data_t data, single_list_node_t **find_node,
                      single_list_node_t **prev_node);
/**
 * @brief find_single_list1 is a simple version of find_single_list.
*/
single_list_node_t *find_single_list1(single_list_t *l, generic_data_t data);

#endif