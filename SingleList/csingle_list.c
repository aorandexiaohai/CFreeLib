#include "csingle_list.h"

#include <assert.h>
#include <stdlib.h>

static single_list_node_t *create_node(generic_data_t data) {
    single_list_node_t *n = calloc(sizeof(single_list_node_t), 1);
    n->data = data;
    return n;
}

single_list_t *create_single_list1(free_function_t free_function, compare_function_t compare_function) {
    single_list_t *l = create_single_list(free_function);
    l->compare_function = compare_function;
    return l;
}

single_list_t *create_single_list(free_function_t free_function) {
    single_list_t *l = calloc(sizeof(single_list_t), 1);
    l->free_function = free_function;
    return l;
}

static void build_relation(single_list_node_t *n1, single_list_node_t *n2) {
    if (n1) {
        n1->next = n2;
    }
}

static single_list_node_t *insert_single_list_at(single_list_t *l, generic_data_t data, single_list_node_t *prev) {
    single_list_node_t *n = create_node(data);

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

/**
 * l->compare_function must be specified.
 */
static void find_the_first_not_less_single_list(single_list_t *l, generic_data_t data, single_list_node_t **find_node,
                                                single_list_node_t **prev_node) {
    single_list_node_t *prev = NULL;
    single_list_node_t *cur = l->head;
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

single_list_node_t *insert_single_list(single_list_t *l, generic_data_t data) {
    single_list_node_t *prev = NULL;
    if (!l->compare_function) {
        prev = l->tail;
    } else {
        single_list_node_t *find_node = 0;
        find_the_first_not_less_single_list(l, data, &find_node, &prev);
    }
    return insert_single_list_at(l, data, prev);
}

single_list_node_t *push_back_single_list(single_list_t *l, generic_data_t data) {
    return insert_single_list_at(l, data, l->tail);
}

generic_data_t pop_front_single_list(single_list_t *l) {
    generic_data_t result = NULL;
    if (l->head) {
        result = l->head->data;
        single_list_node_t *next = l->head->next;
        free(l->head);
        l->head = next;
        if (!l->head) {
            l->tail = NULL;
        }
        l->size--;
    }
    return result;
}

single_list_node_t *find_single_list1(single_list_t *l, generic_data_t data) {
    single_list_node_t *find_node = NULL;
    single_list_node_t *prev_node = NULL;
    find_single_list(l, data, &find_node, &prev_node);
    return find_node;
}
void find_single_list(single_list_t *l, generic_data_t data, single_list_node_t **find_node,
                      single_list_node_t **prev_node) {
    if (l->compare_function) {
        find_the_first_not_less_single_list(l, data, find_node, prev_node);
        if (*find_node && l->compare_function((*find_node)->data, data) != 0) {
            *find_node = 0;
        }
    } else {
        single_list_node_t *prev = NULL;
        single_list_node_t *cur = l->head;
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

static single_list_node_t *find_prev_single_list(single_list_t *l, single_list_node_t *n) {
    single_list_node_t *prev = NULL;
    if (n) {
        single_list_node_t *cur = l->head;
        while (n != cur) {
            prev = cur;
            cur = cur->next;
        }
    }
    return prev;
}

void remove_single_list(single_list_t *l, single_list_node_t *n) {
    if (n) {
        single_list_node_t *prev = find_prev_single_list(l, n);
        build_relation(prev, n->next);
        if (n == l->head) {
            l->head = n->next;
        }
        if (n == l->tail) {
            l->tail = prev;
        }

        if (l->free_function) {
            l->free_function(n->data);
        }
        free(n);
        l->size--;
    }
}

void free_single_list(single_list_t *l) {
    single_list_node_t *cur = l->head;
    while (cur) {
        single_list_node_t *next = cur->next;
        if (l->free_function) {
            l->free_function(cur->data);
        }
        free(cur);
        cur = next;
    }
    free(l);
}