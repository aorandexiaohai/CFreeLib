#include "cheap.h"
#include "carray.h"

#include <stdlib.h>
#include <alloca.h>

void max_heapify(generic_data_t arr, int single_element_size, int element_count,
    data_location_compare_function_t cf, int idx)
{
    for (;;)
    {
        int largest = idx;
        void* a_idx = fetch_element_address(arr, single_element_size, idx);
        void* a_opt = a_idx;

        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        if (left < element_count)
        {
            void* a_left = fetch_element_address(arr, single_element_size, left);
            int cmp = cf(a_left, a_opt);
            if (cmp > 0) {
                largest = left;
                a_opt = a_left;
            }
        }
        if (right < element_count)
        {
            void* a_right = fetch_element_address(arr, single_element_size, right);
            int cmp = cf(a_right, a_opt);
            if (cmp > 0) {
                largest = right;
                a_opt = a_right;
            }
        }
        if (largest != idx)
        {
            void* tmp = alloca(single_element_size);
            swap_content(a_idx, a_opt, tmp, single_element_size);
            idx = largest;
        }
        else
        {
            break;
        }
    }
}

void build_heapify(generic_data_t arr, int single_element_size, int element_count,
    data_location_compare_function_t cf)
{
    int i = 0;
    for (i = element_count / 2 - 1; i >= 0; i--)
    {
        max_heapify(arr, single_element_size, element_count, cf, i);
    }
}