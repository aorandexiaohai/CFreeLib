#include "csort.h"

#include <alloca.h>
#include <stdlib.h>
#include <string.h>

#define ELEMENT_LOC(arr, idx) (void*)((char*)(arr) + (single_element_size) * (idx))

static void sort_array_merge_rec(generic_data_t arr, generic_data_t arr_copy, int single_element_size, int begin,
                                 int end, data_location_compare_function_t cf) {
    if (begin + 1 >= end) return;
    int mid = (begin + end) / 2;
    sort_array_merge_rec(arr, arr_copy, single_element_size, begin, mid, cf);
    sort_array_merge_rec(arr, arr_copy, single_element_size, mid, end, cf);

    int base1 = begin;
    int base2 = mid;
    int base = begin;

    while (base1 < mid && base2 < end) {
        int r = cf(ELEMENT_LOC(arr, base1), ELEMENT_LOC(arr, base2));
        if (r <= 0) {
            memcpy(ELEMENT_LOC(arr_copy, base), ELEMENT_LOC(arr, base1), single_element_size);
            base1++;
        } else {
            memcpy(ELEMENT_LOC(arr_copy, base), ELEMENT_LOC(arr, base2), single_element_size);
            base2++;
        }
        base++;
    }
    while (base1 < mid) {
        memcpy(ELEMENT_LOC(arr_copy, base), ELEMENT_LOC(arr, base1), single_element_size);
        base1++;
        base++;
    }
    while (base2 < end) {
        memcpy(ELEMENT_LOC(arr_copy, base), ELEMENT_LOC(arr, base2), single_element_size);
        base2++;
        base++;
    }

    memcpy(ELEMENT_LOC(arr, begin), ELEMENT_LOC(arr_copy, begin), single_element_size * (end - begin));
}

void sort_array_merge(generic_data_t arr, int single_element_size, int element_count,
                      data_location_compare_function_t cf) {
    generic_data_t arr_copy = malloc(single_element_size * element_count);
    sort_array_merge_rec(arr, arr_copy, single_element_size, 0, element_count, cf);
    free(arr_copy);
}

void sort_array_insertion(generic_data_t arr, int single_element_size, int element_count,
                          data_location_compare_function_t cf) {
    void* key = alloca(single_element_size);
    for (int j = 1; j < element_count; j++) {
        memcpy(key, ELEMENT_LOC(arr, j), single_element_size);
        int i = j - 1;
        while (i >= 0 && cf(ELEMENT_LOC(arr, i), key) > 0) {
            memcpy(ELEMENT_LOC(arr, i + 1), ELEMENT_LOC(arr, i), single_element_size);
            i--;
        }
        memcpy(ELEMENT_LOC(arr, i + 1), key, single_element_size);
    }
}

void sort_array(generic_data_t arr, int single_element_size, int element_count, data_location_compare_function_t cf,
                sort_algorithm_t sa) {
    if (single_element_size <= 1) return;
    if (sa == Merge) {
        sort_array_merge(arr, single_element_size, element_count, cf);
    } else if (sa == Insertion) {
        sort_array_insertion(arr, single_element_size, element_count, cf);
    }
}
