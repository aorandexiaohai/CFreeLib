#ifndef CARRAY_H
#define CARRAY_H
#include "cbase.h"
/**
 * @brief if single_element_size is very large, it may cause the over flow of stack frame. Because when copying
 * intermediate memory, 'alloca' is used, not 'malloc'.
 */
void shuffle_array(generic_data_t arr, int single_element_size, int element_count);
// #define FETCH_ELEMTN_ADDRESS(arr, single_element_size, idx)
static void* fetch_element_address(generic_data_t arr, int single_element_size, int idx) {
    return (void*)((char*)(arr) + (single_element_size) * (idx));
}
/**
 * @brief return a index r and arr[start, r) <= arr[r] <= arr[r+1, end)
*/
int partition(generic_data_t arr, int start, int end, int single_element_size, data_location_compare_function_t cf);
/**
 * @brief swap the content in data_loc1 and data_loc2.
 * 
 * @param data_swap_loc it is a temporary swap memory
*/
void swap_content(generic_data_t data_loc1, generic_data_t data_loc2, generic_data_t data_swap_loc, int element_size);
#endif  // CARRAY_H