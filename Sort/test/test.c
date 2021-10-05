#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "carray.h"
#include "csort.h"

int com_char(generic_data_t data1_loc, generic_data_t data2_loc) {
    unsigned char v1 = *((unsigned char*)data1_loc);
    unsigned char v2 = *((unsigned char*)data2_loc);
    if (v1 < v2) {
        return -1;
    } else if (v1 == v2) {
        return 0;
    }
    return 1;
}

int com_short(generic_data_t data1_loc, generic_data_t data2_loc) {
    short v1 = *((short*)data1_loc);
    short v2 = *((short*)data2_loc);
    if (v1 < v2) {
        return -1;
    } else if (v1 == v2) {
        return 0;
    }
    return 1;
}

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

int com_long(generic_data_t data1_loc, generic_data_t data2_loc) {
    long v1 = *((long*)data1_loc);
    long v2 = *((long*)data2_loc);
    if (v1 < v2) {
        return -1;
    } else if (v1 == v2) {
        return 0;
    }
    return 1;
}

int com_longlong(generic_data_t data1_loc, generic_data_t data2_loc) {
    long long v1 = *((long long*)data1_loc);
    long long v2 = *((long long*)data2_loc);
    if (v1 < v2) {
        return -1;
    } else if (v1 == v2) {
        return 0;
    }
    return 1;
}

typedef int (*func_t)(const void* data1_loc, const void* data2_loc);

#define LENGTH1 100
#define TEST_COUNT 10000
#define PERFORMACE_TEST_COUNT 10
#define MAX_VALUE 127
/**
 * #define ENABLE_PERFORMANCE_TEST
 */
#define ENABLE_PERFORMANCE_TEST

#define GenerateUnitTest(type)                                                                                         \
    {                                                                                                                  \
        for (alg = 0; alg < SortAlgorithmCount; alg++) {                                                               \
            for (i = 0; i < TEST_COUNT; i++) {                                                                         \
                type arr[LENGTH1] = {};                                                                                \
                int len = (rand() % LENGTH1);                                                                          \
                for (k = 0; k < len; k++) {                                                                            \
                    arr[k] = (type)(rand() % (MAX_VALUE + 1));                                                         \
                }                                                                                                      \
                shuffle_array(arr, sizeof(type), len);                                                                 \
                type arr_copy[LENGTH1] = {};                                                                           \
                memcpy(arr_copy, arr, sizeof(type) * len);                                                             \
                if (alg == Count) {                                                                                    \
                    sort_array(arr, sizeof(type), len, (data_location_compare_function_t)((void*)((size_t)MAX_VALUE)), \
                               alg);                                                                                   \
                } else {                                                                                               \
                    sort_array(arr, sizeof(type), len, sort_functions[sizeof(type)], alg);                             \
                }                                                                                                      \
                qsort(arr_copy, len, sizeof(type), (func_t)sort_functions[sizeof(type)]);                              \
                for (k = 0; k < len; k++) {                                                                            \
                    assert(arr[k] == arr_copy[k]);                                                                     \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
        printf("unit test success for %s\n\n-------------------------------------------------------------\n", #type);  \
    }
static data_location_compare_function_t sort_functions[20] = {};
int main() {
    {
        sort_functions[sizeof(unsigned char)] = com_char;
        sort_functions[sizeof(short)] = com_short;
        sort_functions[sizeof(int)] = com_int;
        sort_functions[sizeof(long)] = com_long;
        sort_functions[sizeof(long long)] = com_longlong;
    }
    srand(0);
    int alg = 0;
    int i = 0;
    int k = 0;
    int j = 0;

    GenerateUnitTest(unsigned char);
    GenerateUnitTest(short);
    GenerateUnitTest(int);
    GenerateUnitTest(long);
    GenerateUnitTest(long long);

#if defined(ENABLE_PERFORMANCE_TEST)
    {
        int len = 10000;
        int* arr = malloc(len * sizeof(int));
        int left = len;
        int base = 0;
        while (left > 0) {
            int duplicates = rand() % 10;
            int v = rand() % (MAX_VALUE + 1);
            if (duplicates > left) {
                duplicates = left;
            }
            for (i = 0; i < duplicates; i++) {
                arr[base++] = v;
            }
            left -= duplicates;
        }
        for (alg = 0; alg < SortAlgorithmCount; alg++) {
            struct timeval begin;
            gettimeofday(&begin, NULL);
            for (j = 0; j < PERFORMACE_TEST_COUNT; j++) {
                shuffle_array(arr, sizeof(int), len);
                if (alg == Count) {
                    sort_array(arr, sizeof(int), len, (data_location_compare_function_t)((void*)((size_t)MAX_VALUE)),
                               alg);
                } else {
                    sort_array(arr, sizeof(int), len, com_int, alg);
                }
            }
            struct timeval end;
            gettimeofday(&end, NULL);

            size_t total_us = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
            printf("%20s : %10.2lf ms when sorting %d elements\n", sort_algorithm_strings[alg], total_us / 1.0 / 1000,
                   len);
        }
        free(arr);
        printf("performance test success\n\n");
    }

    {
        int len = 1000000;
        int* arr = malloc(len * sizeof(int));
        int left = len;
        int base = 0;
        while (left > 0) {
            int duplicates = rand() % 10;
            int v = rand() % (MAX_VALUE + 1);
            if (duplicates > left) {
                duplicates = left;
            }
            for (i = 0; i < duplicates; i++) {
                arr[base++] = v;
            }
            left -= duplicates;
        }
        for (alg = 0; alg < SortAlgorithmCount; alg++) {
            if (alg == Bubble || alg == Insertion || alg == Selection) continue;
            struct timeval begin;
            gettimeofday(&begin, NULL);
            for (j = 0; j < PERFORMACE_TEST_COUNT; j++) {
                shuffle_array(arr, sizeof(int), len);
                if (alg == Count) {
                    sort_array(arr, sizeof(int), len, (data_location_compare_function_t)((void*)((size_t)MAX_VALUE)),
                               alg);
                } else {
                    sort_array(arr, sizeof(int), len, com_int, alg);
                }
            }
            struct timeval end;
            gettimeofday(&end, NULL);

            size_t total_us = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
            printf("%20s : %10.2lf ms when sorting %d elements\n", sort_algorithm_strings[alg], total_us / 1.0 / 1000,
                   len);
        }
        free(arr);
        printf("performance test of very large arraies success\n\n");
    }
#endif
    return 0;
}