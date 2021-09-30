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
#endif  // CARRAY_H