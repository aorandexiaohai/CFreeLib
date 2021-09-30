#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "carray.h"
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

#define LENGTH1 100
#define TEST_COUNT 1000
#define PERFORMACE_TEST_COUNT 10
int main() {
    srand(0);
    int alg = 0;
    int i = 0;
    int k = 0;
    int j = 0;
    for (alg = 0; alg < SortAlgorithmCount; alg++) {
        for (i = 0; i < TEST_COUNT; i++) {
            int arr[LENGTH1] = {};
            int len = (rand() % LENGTH1);
            for (k = 0; k < len; k++) {
                arr[k] = k;
            }
            shuffle_array(arr, sizeof(int), len);
            sort_array(arr, sizeof(int), len, com_int, alg);
            for (k = 0; k < len; k++) {
                assert(arr[k] == k);
            }
        }
    }
    printf("unit test success\n\n-------------------------------------------------------------\n");
    int len = 10000;
    int* arr = malloc(len * sizeof(int));
    for (i = 0; i < len; i++) {
        arr[i] = i;
    }
    for (alg = 0; alg < SortAlgorithmCount; alg++) {
        struct timeval begin;
        gettimeofday(&begin, NULL);
        for (j = 0; j < PERFORMACE_TEST_COUNT; j++) {
            shuffle_array(arr, sizeof(int), len);
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