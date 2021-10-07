#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "carray.h"
#include "limits.h"
/**
 * it should be a very large number.
 */
#define SHUFFLE_ARRAY_TEST_COUNT 100000
/**
 * it should be a integer(>1) and it should be a small number.
 */
#define SHUFFLE_ARRAY_LENGTH 5
#define LENGTH 1000

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
    srand(0);
    int i = 0;
    int j = 0;
    {
        int pos_count[SHUFFLE_ARRAY_LENGTH][SHUFFLE_ARRAY_LENGTH] = {};
        int arr[SHUFFLE_ARRAY_LENGTH] = {};
        for (i = 0; i < SHUFFLE_ARRAY_LENGTH; i++) {
            arr[i] = i;
        }
        for (i = 0; i < SHUFFLE_ARRAY_TEST_COUNT; i++) {
            shuffle_array(arr, sizeof(int), SHUFFLE_ARRAY_LENGTH);
            for (j = 0; j < SHUFFLE_ARRAY_LENGTH; j++) {
                pos_count[j][arr[j]]++;
            }
        }
        for (i = 0; i < SHUFFLE_ARRAY_LENGTH; i++) {
            int minv = INT_MAX;
            int maxv = INT_MIN;

            for (j = 0; j < SHUFFLE_ARRAY_LENGTH; j++) {
                if (minv > pos_count[i][j]) {
                    minv = pos_count[i][j];
                }
                if (maxv < pos_count[i][j]) {
                    maxv = pos_count[i][j];
                }
            }
            assert(maxv - minv <= SHUFFLE_ARRAY_TEST_COUNT / SHUFFLE_ARRAY_LENGTH);
        }
    }
    {
        int arr[LENGTH] = {0};
        for (i = 0; i < LENGTH; i++) {
            arr[i] = i;
        }
        int len = 0;
        for (len = 1; len < LENGTH; len++) {
            for (i = 0; i < len; i++) {
                int result = 0;
                random_select(arr, sizeof(int), len, com_int, i, &result);
                assert(result == i);
            }
        }
    }
    printf("test success\n");
    return 0;
}