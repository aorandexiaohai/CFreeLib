#include "carray.h"

#include <alloca.h>
#include <stdlib.h>
#include <string.h>

void shuffle_array(generic_data_t arr, int single_element_size, int element_count) {
    void* tmp = alloca(single_element_size);
    for (int i = element_count - 1; i >= 1; i--) {
        void* ai = fetch_element_address(arr, single_element_size, i);
        int j = rand() % (i + 1);
        void* aj = fetch_element_address(arr, single_element_size, j);
        memcpy(tmp, ai, single_element_size);
        memcpy(ai, aj, single_element_size);
        memcpy(aj, tmp, single_element_size);
    }
}