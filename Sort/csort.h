#ifndef CSORT_H
#define CSORT_H
#include "cbase.h"
/**
 * sort consecutive array by different algorithms.
*/
enum sort_algorithm_inner {
    Merge,
    Insertion
};
typedef enum sort_algorithm_inner sort_algorithm_t;
void sort_array(generic_data_t arr, int element_length, int element_count, compare_function_t cf, sort_algorithm_t sa);
#endif//CSORT_H