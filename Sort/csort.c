#include "csort.h"

#include <alloca.h>
#include <assert.h>
#include <string.h>

#include "carray.h"
#include "cheap.h"
#define ELEMENT_LOC(arr, idx) fetch_element_address(arr, single_element_size, idx)

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
        void* addr1 = ELEMENT_LOC(arr, base1);
        void* addr2 = ELEMENT_LOC(arr, base2);
        void* addr = ELEMENT_LOC(arr_copy, base);
        int r = cf(addr1, addr2);
        if (r <= 0) {
            memcpy(addr, addr1, single_element_size);
            base1++;
        } else {
            memcpy(addr, addr2, single_element_size);
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
#include <stdio.h>
static void sort_array_insertion_gap(generic_data_t arr, int single_element_size, int element_count,
                                     data_location_compare_function_t cf, int gap) {
    void* key = alloca(single_element_size);
    int j = 0;
    for (j = gap; j < element_count; j++) {
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
    int i = 0, j = 0;
    for (i = 0; i + 1 < element_count; i++) {
        for (j = 0; j + i + 1 < element_count; j++) {
            generic_data_t aj = ELEMENT_LOC(arr, j);
            generic_data_t aj1 = ELEMENT_LOC(arr, j + 1);
            if (cf(aj, aj1) > 0) {
                swap_content(aj1, aj, tmp, single_element_size);
            }
        }
    }
}

static void sort_array_shell(generic_data_t arr, int single_element_size, int element_count,
                             data_location_compare_function_t cf) {
    int gap = 0;
    for (gap = element_count / 2; gap > 0; gap /= 2) {
        sort_array_insertion_gap(arr, single_element_size, element_count, cf, gap);
    }
}

static void sort_array_selection(generic_data_t arr, int single_element_size, int element_count,
                                 data_location_compare_function_t cf) {
    void* tmp = alloca(single_element_size);
    int i = 0, j = 0;
    for (i = 0; i + 1 < element_count; i++) {
        generic_data_t ai = ELEMENT_LOC(arr, i);
        for (j = i + 1; j < element_count; j++) {
            generic_data_t aj = ELEMENT_LOC(arr, j);
            if (cf(ai, aj) > 0) {
                swap_content(ai, aj, tmp, single_element_size);
            }
        }
    }
}

static void sort_array_quick_rec(generic_data_t arr, int single_element_size, int p, int r,
                                 data_location_compare_function_t cf) {
    if (p + 1 >= r) return;
    int q = partition(arr, p, r, single_element_size, cf);
    sort_array_quick_rec(arr, single_element_size, p, q, cf);
    sort_array_quick_rec(arr, single_element_size, q + 1, r, cf);
}

static void sort_array_quick(generic_data_t arr, int single_element_size, int element_count,
                             data_location_compare_function_t cf) {
    sort_array_quick_rec(arr, single_element_size, 0, element_count, cf);
}

static void sort_array_quick3way_rec(generic_data_t arr, int single_element_size, int p, int r,
                                     data_location_compare_function_t cf) {
    if (p + 1 >= r) return;
    int left = 0;
    int right = 0;
    partition1(arr, p, r, single_element_size, cf, &left, &right);
    sort_array_quick3way_rec(arr, single_element_size, p, left, cf);
    sort_array_quick3way_rec(arr, single_element_size, right + 1, r, cf);
}

static void sort_array_quick3way(generic_data_t arr, int single_element_size, int element_count,
                                 data_location_compare_function_t cf) {
    sort_array_quick3way_rec(arr, single_element_size, 0, element_count, cf);
}

static void sort_array_heap(generic_data_t arr, int single_element_size, int element_count,
                            data_location_compare_function_t cf) {
    build_heapify(arr, single_element_size, element_count, cf);
    void* tmp = alloca(single_element_size);
    void* a_0 = fetch_element_address(arr, single_element_size, 0);
    int i = 0;
    for (i = element_count - 1; i >= 1; i--) {
        void* a_i = fetch_element_address(arr, single_element_size, i);
        swap_content(a_0, a_i, tmp, single_element_size);
        max_heapify(arr, single_element_size, i, cf, 0);
    }
}

#define EXTEND_FOR_TYPE(type)                                                                                \
    type* arr = (type*)cmp_arr;                                                                              \
    type* output_arr = (type*)cmp_arr_out;                                                                   \
    int i = 0;                                                                                               \
    for (i = 0; i < element_count; i++) {                                                                    \
        count_array[arr[i]]++;                                                                               \
    }                                                                                                        \
    for (i = 1; i <= max_value; i++) {                                                                       \
        count_array[i] += count_array[i - 1];                                                                \
    }                                                                                                        \
    if (!satellite_arr) {                                                                                    \
        for (i = element_count - 1; i >= 0; i--) {                                                           \
            output_arr[count_array[arr[i]] - 1] = arr[i];                                                    \
            count_array[arr[i]]--;                                                                           \
        }                                                                                                    \
    } else {                                                                                                 \
        for (i = element_count - 1; i >= 0; i--) {                                                           \
            int j = count_array[arr[i]] - 1;                                                                 \
            output_arr[j] = arr[i];                                                                          \
            memcpy(fetch_element_address(output_satellite_arr, satellite_element_size, j),                   \
                   fetch_element_address(satellite_arr, satellite_element_size, i), satellite_element_size); \
            count_array[arr[i]]--;                                                                           \
        }                                                                                                    \
    }

static void count_sort_array_satellite(generic_data_t cmp_arr, generic_data_t cmp_arr_out, generic_data_t satellite_arr,
                                       generic_data_t* output_satellite_arr, int cmp_element_size,
                                       int satellite_element_size, int element_count, size_t max_value,
                                       int* count_array) {
    if (cmp_element_size == sizeof(unsigned char)) {
        EXTEND_FOR_TYPE(unsigned char);
    } else if (cmp_element_size == sizeof(unsigned short)) {
        EXTEND_FOR_TYPE(unsigned short);
    } else if (cmp_element_size == sizeof(unsigned int)) {
        EXTEND_FOR_TYPE(unsigned int);
    } else if (cmp_element_size == sizeof(unsigned long)) {
        EXTEND_FOR_TYPE(unsigned long);
    } else if (cmp_element_size == sizeof(unsigned long long)) {
        EXTEND_FOR_TYPE(unsigned long long);
    }
    memcpy(cmp_arr, cmp_arr_out, cmp_element_size * element_count);
    if (satellite_arr) {
        memcpy(satellite_arr, output_satellite_arr, satellite_element_size * element_count);
    }
}

void count_sort_array(generic_data_t cmp_arr, int single_element_size, int element_count, size_t max_value) {
    int* count_array = calloc(sizeof(int) * (max_value + 1), 1);
    generic_data_t* cmp_arr_out = malloc(single_element_size * element_count);
    count_sort_array_satellite(cmp_arr, cmp_arr_out, NULL, NULL, single_element_size, 0, element_count, max_value,
                               count_array);
    free(count_array);
    free(cmp_arr_out);
}

static void sort_array_count(generic_data_t arr, int single_element_size, int element_count,
                             data_location_compare_function_t cf) {
    count_sort_array(arr, single_element_size, element_count, (size_t)((void*)cf));
}

void radix_sort_array(generic_data_t arr, int single_element_size, int element_count) {
    int total_size = single_element_size * element_count;
    unsigned char* cmp_arr = malloc(sizeof(unsigned char) * element_count);
    const int max_value = 255;
    unsigned char* cmp_arr_out = malloc(sizeof(unsigned char) * element_count);
    int* count_array = malloc(sizeof(int) * (max_value + 1));
    generic_data_t output_satellite_arr = malloc(total_size);
    int i = 0;
    int j = 0;
    int k = 0;
    {
        if (is_small_endian()) {
            for (i = 0; i < single_element_size; i++) {
                for (j = 0; j < element_count; j++) {
                    cmp_arr[j] = ((unsigned char*)arr)[j * single_element_size + i];
                }
                memset(count_array, 0, sizeof(int) * (max_value + 1));
                count_sort_array_satellite(cmp_arr, cmp_arr_out, arr, output_satellite_arr, sizeof(unsigned char),
                                           single_element_size, element_count, max_value, count_array);
            }
        } else {
            for (i = 0; i < single_element_size; i++) {
                for (j = 0; j < element_count; j++) {
                    cmp_arr[j] = ((unsigned char*)arr)[j * single_element_size + single_element_size - 1 - i];
                }
                memset(count_array, 0, sizeof(int) * (max_value + 1));
                count_sort_array_satellite(cmp_arr, cmp_arr_out, arr, output_satellite_arr, sizeof(unsigned char),
                                           single_element_size, element_count, max_value, count_array);
            }
        }
    }
    free(count_array);
    free(cmp_arr_out);
    free(cmp_arr);
    free(output_satellite_arr);
}

static void sort_array_radix(generic_data_t arr, int single_element_size, int element_count,
                             data_location_compare_function_t cf) {
    radix_sort_array(arr, single_element_size, element_count);
}

typedef void (*sort_array_inner_t)(generic_data_t arr, int single_element_size, int element_count,
                                   data_location_compare_function_t cf);

void sort_array(generic_data_t arr, int single_element_size, int element_count, data_location_compare_function_t cf,
                sort_algorithm_t sa) {
    if (element_count <= 1) return;
    assert(0 <= sa && sa < SortAlgorithmCount);
    static sort_array_inner_t funcs[] = {
        sort_array_insertion, sort_array_bubble,    sort_array_merge, sort_array_shell, sort_array_selection,
        sort_array_quick,     sort_array_quick3way, sort_array_heap,  sort_array_count, sort_array_radix};
    return funcs[sa](arr, single_element_size, element_count, cf);
}