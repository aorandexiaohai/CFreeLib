#ifndef CBASE_H
#define CBASE_H
#include <stdbool.h>

typedef void *generic_data_t;
typedef void (*free_function_t)(generic_data_t);
typedef int (*compare_function_t)(generic_data_t data1, generic_data_t data2);
typedef int (*data_location_compare_function_t)(generic_data_t data1_location, generic_data_t data2_location);
typedef generic_data_t (*generic_function_t)(generic_data_t);
/**
 * @brief swap the content in data_loc1 and data_loc2.
 *
 * @param data_swap_loc it is a temporary swap memory
 */
void swap_content(generic_data_t data_loc1, generic_data_t data_loc2, generic_data_t data_swap_loc, int element_size);

static bool is_small_endien() {
    int i = 1;
    return *((char *)(&i));
}
#endif