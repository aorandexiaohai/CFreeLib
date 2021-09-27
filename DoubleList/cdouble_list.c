#include "cdouble_list.h"

#include <assert.h>
#include <stdlib.h>

static double_list_node_t *create_node(generic_data_t data) {
    double_list_node_t *n = calloc(sizeof(double_list_node_t), 1);
    n->data = data;
    return n;
}

double_list_t *create_double_list(free_function_t free_function) {
    double_list_t *l = calloc(sizeof(double_list_t), 1);
    l->free_function = free_function;
    return l;
}
double_list_t *create_double_list1(free_function_t free_function, compare_function_t compare_function) {
    double_list_t *l = create_double_list(free_function);
    l->compare_function = compare_function;
    return l;
}

static void build_relation(double_list_node_t *n1, double_list_node_t *n2) {
    if (n1) {
        n1->next = n2;
    }
    if (n2) {
        n2->prev = n1;
    }
}


generic_data_t pop_front_double_list(double_list_t *l) {
    generic_data_t result = NULL;
    if (l->head) {
        result = l->head->data;
        double_list_node_t *next = l->head->next;
        build_relation(l->head, next);
        free(l->head);
        l->head = next;
        if (!l->head) {
            l->tail = next;
        }
        l->size--;
    }
    return result;
}

void remove_double_list(double_list_t *l, double_list_node_t *n) {
    if (n) {
        build_relation(n->prev, n->next);
        if (n == l->head) {
            l->head = n->next;
        }
        if (n == l->tail) {
            l->tail = n->prev;
        }

        if (l->free_function) {
            l->free_function(n->data);
        }
        free(n);
        l->size--;
    }
}

void free_double_list(double_list_t *l) {
    double_list_node_t *cur = l->head;
    while (cur) {
        double_list_node_t *next = cur->next;
        if (l->free_function) {
            l->free_function(cur->data);
        }
        free(cur);
        cur = next;
    }
    free(l);
}

/**
 * l->compare_function must be specified.
 */
static void find_the_first_not_less_double_list(double_list_t *l, generic_data_t data, double_list_node_t **find_node,
                                                double_list_node_t **prev_node) {
    double_list_node_t *prev = NULL;
    double_list_node_t *cur = l->head;
    assert(l->compare_function);
    while (cur) {
        if (l->compare_function(cur->data, data) >= 0) {
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    *find_node = cur;
    *prev_node = prev;
}

double_list_node_t *find_double_list1(double_list_t *l, generic_data_t data) {
    double_list_node_t *find_node = NULL;
    double_list_node_t *prev_node = NULL;
    find_double_list(l, data, &find_node, &prev_node);
    return find_node;
}
void find_double_list(double_list_t *l, generic_data_t data, double_list_node_t **find_node,
                      double_list_node_t **prev_node) {
    if (l->compare_function) {
        find_the_first_not_less_double_list(l, data, find_node, prev_node);
        if (*find_node && l->compare_function((*find_node)->data, data) != 0) {
            *find_node = 0;
        }
    } else {
        double_list_node_t *prev = NULL;
        double_list_node_t *cur = l->head;
        while (cur) {
            if (data == cur->data) {
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        *find_node = cur;
        *prev_node = prev;
    }
}

static double_list_node_t *insert_double_list_at(double_list_t *l, generic_data_t data, double_list_node_t *prev) {
    double_list_node_t *n = create_node(data);

    if (prev) {
        build_relation(n, prev->next);
        build_relation(prev, n);

    } else {
        build_relation(n, l->head);
        l->head = n;
    }
    if (l->tail == prev) {
        l->tail = n;
    }
    l->size++;
    return n;
}

double_list_node_t *insert_double_list(double_list_t *l, generic_data_t data) {
    double_list_node_t *prev = NULL;
    if (!l->compare_function) {
        prev = l->tail;
    } else {
        double_list_node_t *find_node = 0;
        find_the_first_not_less_double_list(l, data, &find_node, &prev);
    }
    return insert_double_list_at(l, data, prev);
}

double_list_node_t *push_back_double_list(double_list_t *l, generic_data_t data) {
    return insert_double_list_at(l, data, l->tail);
}