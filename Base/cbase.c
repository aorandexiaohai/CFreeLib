#include "cbase.h"
#include <string.h>

void swap_content(generic_data_t data_loc1, generic_data_t data_loc2, generic_data_t tmp, int element_size) {
    if (data_loc1 == data_loc2) return;
    memcpy(tmp, data_loc1, element_size);
    memcpy(data_loc1, data_loc2, element_size);
    memcpy(data_loc2, tmp, element_size);
}