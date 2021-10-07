#include "carray.h"

#include <alloca.h>
#include <stdlib.h>
#include <string.h>

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
    void* tmp = alloca(single_element_size);
    
    {
        /**
         * use a random pivot.
         */
        int i = rand() % (r - p + 1) + p;
        swap_content(fetch_element_address(arr, single_element_size, i), ar, tmp, single_element_size);
    }

    memcpy(x, ar, single_element_size);
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

void partition1(generic_data_t arr, int lo, int hi, int single_element_size, data_location_compare_function_t cf,
                int* lt, int* gt) {
    if (lo + 1 >= hi) return;
    hi--;
    *lt = lo;
    *gt = hi;
    void* v = alloca(single_element_size);
    void* tmp = alloca(single_element_size);
    memcpy(v, fetch_element_address(arr, single_element_size, lo), single_element_size);
    int i = lo;
    while (i <= *gt) {
        void* ai = fetch_element_address(arr, single_element_size, i);
        int cmp = cf(ai, v);
        if (cmp < 0) {
            swap_content(ai, fetch_element_address(arr, single_element_size, *lt), tmp, single_element_size);
            i++;
            (*lt)++;
        } else if (cmp > 0) {
            swap_content(ai, fetch_element_address(arr, single_element_size, *gt), tmp, single_element_size);
            (*gt)--;
        } else {
            i++;
        }
    }
}

void random_select(generic_data_t arr, int single_element_size, int element_count, data_location_compare_function_t cf,
                   int i, generic_data_t result) {
    if (i >= element_count || i < 0) {
        return;
    }
    int p = 0;
    int r = element_count;
    while (true) {
        int q = partition(arr, p, r, single_element_size, cf);
        int k = q - p;
        if (i == k) {
            memcpy(result, fetch_element_address(arr, single_element_size, q), single_element_size);
            break;
        } else if (i < k) {
            r = q;
        } else {
            p = q + 1;
            i -= k + 1;
        }
    }
}