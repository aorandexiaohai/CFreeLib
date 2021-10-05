#ifndef CSORT_H
#define CSORT_H
#include <stdlib.h>

#include "cbase.h"
/**
 * sort consecutive array by different algorithms.
 */
enum sort_algorithm_inner {
    Insertion = 0,
    Bubble,
    Merge,
    Shell,
    Selection,
    Quick,
    Quick3Way,
    Heap,
    Count,
    Radix,
    SortAlgorithmCount
};
static const char* sort_algorithm_strings[] = {"Insertion", "Bubble",    "Merge", "Shell", "Selection",
                                               "Quick",     "Quick3Way", "Heap",  "Count", "Radix"};
typedef enum sort_algorithm_inner sort_algorithm_t;
/**
 * @brief if single_element_size is very large, it may cause the over flow of stack frame. Because when copying
 * intermediate memory, 'alloca' is used, not 'malloc'. For count sort, 'cf' is a converted value of 'max_value'. For
 * radix sort, 'cf' is useless.
 * @see count_sort_array
 */
void sort_array(generic_data_t arr, int single_element_size, int element_count, data_location_compare_function_t cf,
                sort_algorithm_t sa);

/**
 * single_element_size only supports:   1(unsigned char/signed char),2(unsigned short/signed short),4(unsigned
 * int/signed short),8(unsigned long/singed long) if you want to used the sign's version, please ensure that all values
 * are positive.
 */
void count_sort_array(generic_data_t arr, int single_element_size, int element_count, size_t max_value);
void radix_sort_array(generic_data_t arr, int single_element_size, int element_count);
#endif