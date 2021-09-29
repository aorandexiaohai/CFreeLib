#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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
#define LENGTH1 100
#define TEST_COUNT 500
#define PERFORMACE_TEST_COUNT 10
int main() {
    srand(0);
    for (int alg = 0; alg < SortAlgorithmCount; alg++) {
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
    printf("unit test success\n\n");
    int len = 10000;
    int* arr = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) {
        arr[i] = i;
    }
    for (int alg = 0; alg < SortAlgorithmCount; alg++) {
        struct timeval begin;
        gettimeofday(&begin, NULL);
        for (int j = 0; j < PERFORMACE_TEST_COUNT; j++) {
            shuffle_array(arr, len);
            sort_array(arr, sizeof(int), len, com_int, alg);
        }
        struct timeval end;
        gettimeofday(&end, NULL);

        size_t total_us = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
        printf("%20s : %10.2lf ms when sorting %d elements\n", sort_algorithm_strings[alg], total_us / 1.0 / 1000, len);
    }
    free(arr);
    printf("performance test success\n\n");
    return 0;
}