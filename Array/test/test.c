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

int main() {
    srand(0);
    {
        int pos_count[SHUFFLE_ARRAY_LENGTH][SHUFFLE_ARRAY_LENGTH] = {};
        int arr[SHUFFLE_ARRAY_LENGTH] = {};
        int i = 0;
        int j = 0;
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
    printf("test success\n");
    return 0;
}