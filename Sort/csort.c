#include "csort.h"

void sort_array_merge(generic_data_t arr, int single_element_length, int element_count, compare_function_t cf) {}

void sort_array_insertion(generic_data_t arr, int single_element_length, int element_count, compare_function_t cf) {}

void sort_array(generic_data_t arr, int single_element_length, int element_count, compare_function_t cf,
                sort_algorithm_t sa) {
    if (sa == Merge) {
        sort_array_merge(arr, single_element_length, element_count, cf);
    } else if (sa == Insertion) {
        sort_array_insertion(arr, single_element_length, element_count, cf);
    }
}
