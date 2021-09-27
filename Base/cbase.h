#ifndef CBASE_H
#define CBASE_H
#include <stdbool.h>

typedef void *generic_data_t;
typedef void (*free_function_t)(generic_data_t);
typedef int (*compare_function_t)(generic_data_t data1, generic_data_t data2);
typedef int (*data_location_compare_function_t)(generic_data_t data1_location, generic_data_t data2_location);
typedef generic_data_t (*generic_function_t)(generic_data_t);
#endif