#ifndef CARRAY_H
#define CARRAY_H
#include "cbase.h"
/**
 * @brief if single_element_size is very large, it may cause the over flow of stack frame. Because when copying
 * intermediate memory, 'alloca' is used, not 'malloc'.
 */
void shuffle_array(generic_data_t arr, int single_element_size, int element_count);

static void* fetch_element_address(generic_data_t arr, int single_element_size, int idx) {
    return (void*)((char*)(arr)+(single_element_size) * (idx));
}
/**
 * @brief return a index r and arr[start, r) <= arr[r] <= arr[r+1, end)
*/
int partition(generic_data_t arr, int start, int end, int single_element_size, data_location_compare_function_t cf);

/**
 * @brief set r,l and arr[start, r) <= arr[r] = arr[r+1] = ... = arr[l-1] = arr[l] <= arr[l+1, end)
*/
void partition1(generic_data_t arr, int start, int end, int single_element_size, data_location_compare_function_t cf, int* left, int* right);


#endif 