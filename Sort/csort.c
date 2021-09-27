#include "csort.h"

#include <string.h>

#define ARR(idx) (void*)((char*)(arr) + (single_element_size) * (idx))

void sort_array_merge(generic_data_t arr, int single_element_size, int element_count, compare_function_t cf) {}

void sort_array_insertion(generic_data_t arr, int single_element_size, int element_count, compare_function_t cf) {
    for (int j = 1; j < element_count; j++) {
        void* key = ARR(j);
        int i = j - 1;
        while (i >= 0 && cf(ARR(i), key) > 0) {
            memcpy(ARR(i + 1), ARR(i), single_element_size);
            i--;
        }
        memcpy(ARR(i + 1), key, single_element_size);
    }
}

void sort_array(generic_data_t arr, int single_element_size, int element_count, compare_function_t cf,
                sort_algorithm_t sa) {
    if (sa == Merge) {
        sort_array_merge(arr, single_element_size, element_count, cf);
    } else if (sa == Insertion) {
        sort_array_insertion(arr, single_element_size, element_count, cf);
    }
}
