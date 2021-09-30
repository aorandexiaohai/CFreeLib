#include "csort.h"

#include <alloca.h>
#include <assert.h>
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

static void sort_array_merge(generic_data_t arr, int single_element_size, int element_count,
                             data_location_compare_function_t cf) {
    generic_data_t arr_copy = malloc(single_element_size * element_count);
    sort_array_merge_rec(arr, arr_copy, single_element_size, 0, element_count, cf);
    free(arr_copy);
}

static void sort_array_insertion_gap(generic_data_t arr, int single_element_size, int element_count,
                                     data_location_compare_function_t cf, int gap) {
    void* key = alloca(single_element_size);
    for (int j = gap; j < element_count; j++) {
        memcpy(key, ELEMENT_LOC(arr, j), single_element_size);
        int i = j - gap;
        while (i >= 0 && cf(ELEMENT_LOC(arr, i), key) > 0) {
            memcpy(ELEMENT_LOC(arr, i + gap), ELEMENT_LOC(arr, i), single_element_size);
            i -= gap;
        }
        memcpy(ELEMENT_LOC(arr, i + gap), key, single_element_size);
    }
}

static void sort_array_insertion(generic_data_t arr, int single_element_size, int element_count,
                                 data_location_compare_function_t cf) {
    sort_array_insertion_gap(arr, single_element_size, element_count, cf, 1);
}
static void sort_array_bubble(generic_data_t arr, int single_element_size, int element_count,
                              data_location_compare_function_t cf) {
    void* tmp = alloca(single_element_size);
    for (int i = 0; i + 1 < element_count; i++) {
        for (int j = 0; j + i + 1 < element_count; j++) {
            generic_data_t aj = ELEMENT_LOC(arr, j);
            generic_data_t aj1 = ELEMENT_LOC(arr, j + 1);
            if (cf(aj, aj1) > 0) {
                memcpy(tmp, aj, single_element_size);
                memcpy(aj, aj1, single_element_size);
                memcpy(aj1, tmp, single_element_size);
            }
        }
    }
}

static void sort_array_shell(generic_data_t arr, int single_element_size, int element_count,
                             data_location_compare_function_t cf) {
    for (int gap = element_count / 2; gap > 0; gap /= 2) {
        sort_array_insertion_gap(arr, single_element_size, element_count, cf, gap);
    }
}

static void sort_array_selection(generic_data_t arr, int single_element_size, int element_count,
                                 data_location_compare_function_t cf) {
    void* tmp = alloca(single_element_size);
    for (int i = 0; i + 1 < element_count; i++) {
        generic_data_t ai = ELEMENT_LOC(arr, i);
        for (int j = i + 1; j < element_count; j++) {
            generic_data_t aj = ELEMENT_LOC(arr, j);
            if (cf(ai, aj) > 0) {
                memcpy(tmp, ai, single_element_size);
                memcpy(ai, aj, single_element_size);
                memcpy(aj, tmp, single_element_size);
            }
        }
    }
}

typedef void (*sort_array_inner_t)(generic_data_t arr, int single_element_size, int element_count,
                                   data_location_compare_function_t cf);

void sort_array(generic_data_t arr, int single_element_size, int element_count, data_location_compare_function_t cf,
                sort_algorithm_t sa) {
    if (single_element_size <= 1) return;
    assert(0 <= sa && sa < SortAlgorithmCount);
    static sort_array_inner_t funcs[] = {sort_array_insertion, sort_array_bubble, sort_array_merge, sort_array_shell,
                                         sort_array_selection};
    return funcs[sa](arr, single_element_size, element_count, cf);
}
