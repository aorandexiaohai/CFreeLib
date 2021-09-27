#include <stdio.h>
#include <stdlib.h>

#include "csort.h"

int com_int(generic_data_t data1_loc, generic_data_t data2_loc) {
    int v1 = *((int*)data1_loc);
    int v2 = *((int*)data2_loc);
    if (v1 < v2) {
        return -1;
    } else if (v1 == v2) {
        return 0;
    }
    return 1;
}

int main() {
    {
#define LENGTH1 20
        int arr[LENGTH1] = {};
        for (int i = 0; i < LENGTH1; i++) {
            arr[i] = rand() % LENGTH1;
        }
        for (int i = 0; i < LENGTH1; i++) {
            printf("%3d ", arr[i]);
        }
        printf("\n");
        sort_array(arr, sizeof(int), LENGTH1, com_int, Insertion);
        for (int i = 0; i < LENGTH1; i++) {
            printf("%3d ", arr[i]);
        }
        printf("\n");
    }
    return 0;
}