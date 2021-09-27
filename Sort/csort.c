#include "csort.h"

void sort_array_merge(generic_data_t arr, int element_length, int element_count, compare_function_t cf) 
{
}

void sort_array_insertion(generic_data_t arr, int element_length, int element_count, compare_function_t cf) 
{
}

void sort_array(generic_data_t arr, int element_length, int element_count, compare_function_t cf, sort_algorithm_t sa) 
{
    if(sa == sort_algorithm_t::Merge) {
        sort_array_merge(arr, element_length, element_count, cf);
    } else if(sa == sort_algorithm_t::Insertion) {
        sort_array_insertion(arr, element_length, element_count, cf);
    }
}

