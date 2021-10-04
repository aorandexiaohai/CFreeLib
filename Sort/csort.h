#ifndef CSORT_H
#define CSORT_H
#include "cbase.h"
/**
 * sort consecutive array by different algorithms.
 */
enum sort_algorithm_inner { Insertion = 0, Bubble, Merge, Shell, Selection, Quick, Quick3Way, SortAlgorithmCount };
static const char* sort_algorithm_strings[] = { "Insertion", "Bubble", "Merge", "Shell", "Selection", "Quick", "Quick3Way" };
typedef enum sort_algorithm_inner sort_algorithm_t;
/**
 * @brief if single_element_size is very large, it may cause the over flow of stack frame. Because when copying
 * intermediate memory, 'alloca' is used, not 'malloc'.
 */
void sort_array(generic_data_t arr, int single_element_size, int element_count, data_location_compare_function_t cf,
    sort_algorithm_t sa);
#endif