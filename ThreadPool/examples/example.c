#define _XOPEN_SOURCE 500

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cthread_pool.h"

void* print_hello(void* args) {
    usleep(rand() % 100000);
    printf("%s\n", "hello");
    return 0;
}

void* calculate_square(void* args) {
    usleep(rand() % 100000);
    size_t x = (size_t)args;
    return (void*)(x * x);
}

int main() {
    size_t i = 0;
    {
        printf("test1\n");
        printf("-----------------------------------\n");
        thread_pool_t* p = create_thread_pool(4, 0);
        void* dummy_address = NULL;
        future_t* fut1 = enqueue_task_thread_pool(p, print_hello, 0, &dummy_address);
        future_t* fut2 = enqueue_task_thread_pool(p, print_hello, 0, &dummy_address);
        future_t* fut3 = enqueue_task_thread_pool(p, print_hello, 0, &dummy_address);
        wait_and_close_future(fut1);
        wait_and_close_future(fut2);
        wait_and_close_future(fut3);
        close_thread_pool(p, false);
        printf("\n***********************************\n");
    }

    {
        printf("test2\n");
        printf("-----------------------------------\n");
        thread_pool_t* p = create_thread_pool(4, 0);
        enqueue_task_thread_pool(p, print_hello, 0, 0);
        enqueue_task_thread_pool(p, print_hello, 0, 0);
        enqueue_task_thread_pool(p, print_hello, 0, 0);
        close_thread_pool(p, true);
        printf("\n***********************************\n");
    }

    {
        printf("test3\n");
        printf("-----------------------------------\n");
        thread_pool_t* p = create_thread_pool(4, 0);
        void *r1, *r2, *r3;
        future_t* fut1 = enqueue_task_thread_pool(p, calculate_square, (void*)(size_t)1, &r1);
        future_t* fut2 = enqueue_task_thread_pool(p, calculate_square, (void*)(size_t)2, &r2);
        future_t* fut3 = enqueue_task_thread_pool(p, calculate_square, (void*)(size_t)3, &r3);
        wait_and_close_future(fut1);
        wait_and_close_future(fut2);
        wait_and_close_future(fut3);
        assert((size_t)r1 == 1);
        assert((size_t)r2 == 4);
        assert((size_t)r3 == 9);
        close_thread_pool(p, true);
        printf("\n***********************************\n");
    }

    {
        printf("test4\n");
        printf("-----------------------------------\n");
        thread_pool_t* p = create_thread_pool(2, 1);
        future_t* futs[100] = {0};
        void* rs[100] = {0};

        for (i = 0; i < 100; i++) {
            futs[i] = enqueue_task_thread_pool(p, calculate_square, (void*)i, &rs[i]);
        }
        for (i = 0; i < 100; i++) {
            wait_and_close_future(futs[i]);
            assert((size_t)rs[i] == i * i);
        }
        close_thread_pool(p, true);
        printf("\n***********************************\n");
    }
    return 0;
}