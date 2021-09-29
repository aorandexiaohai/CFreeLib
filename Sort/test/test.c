#include <assert.h>
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

static void shuffle_array(int* arr, int size) {
    for (int i = size - 1; i >= 1; i--) {
        int back_idx = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[back_idx];
        arr[back_idx] = tmp;
    }
}
#define LENGTH1 1000
#define TEST_COUNT 100
int main() {
    for(int alg = 0; alg<SortAlgorithmCount; alg++)
    {
        for (int i = 0; i < TEST_COUNT; i++) {
            int arr[LENGTH1] = {};
            int len = (rand() % LENGTH1);
            for (int i = 0; i < len; i++) {
                arr[i] = i;
            }
            shuffle_array(arr, len);
            sort_array(arr, sizeof(int), len, com_int, alg);
            for (int i = 0; i < len; i++) {
                assert(arr[i] == i);
            }
        }
    }

    printf("test success\n");
    return 0;
}