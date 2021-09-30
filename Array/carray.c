#include "carray.h"

#include <alloca.h>
#include <stdlib.h>
#include <string.h>

void swap_content(generic_data_t data_loc1, generic_data_t data_loc2, generic_data_t tmp, int element_size) {
    if (data_loc1 == data_loc2) return;
    memcpy(tmp, data_loc1, element_size);
    memcpy(data_loc1, data_loc2, element_size);
    memcpy(data_loc2, tmp, element_size);
}

void shuffle_array(generic_data_t arr, int single_element_size, int element_count) {
    void* tmp = alloca(single_element_size);
    int i = 0;
    for (i = element_count - 1; i >= 1; i--) {
        int j = rand() % (i + 1);
        swap_content(fetch_element_address(arr, single_element_size, i),
                     fetch_element_address(arr, single_element_size, j), tmp, single_element_size);
    }
}

int partition(generic_data_t arr, int p, int r, int single_element_size, data_location_compare_function_t cf) {
    if (p + 1 >= r) return p;
    r--;
    void* x = alloca(single_element_size);
    void* ar = fetch_element_address(arr, single_element_size, r);
    memcpy(x, ar, single_element_size);

    void* tmp = alloca(single_element_size);

    int i = p - 1;
    int j = 0;
    for (j = p; j < r; j++) {
        void* aj = fetch_element_address(arr, single_element_size, j);
        if (cf(aj, x) <= 0) {
            i++;
            swap_content(fetch_element_address(arr, single_element_size, i), aj, tmp, single_element_size);
        }
    }
    swap_content(fetch_element_address(arr, single_element_size, i + 1), ar, tmp, single_element_size);

    return i + 1;
}