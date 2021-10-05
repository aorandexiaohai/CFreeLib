#ifndef CHEAP_H
#define cheap_h
/**
 * it is an implementation of max heap.
 * In a max heap, the parent should not be less than its children.
*/

#include "cbase.h"

void max_heapify(generic_data_t arr, int single_element_size, int element_count,
    data_location_compare_function_t cf, int idx);
void build_heapify(generic_data_t arr, int single_element_size, int element_count,
    data_location_compare_function_t cf);

#endif