#include <stdio.h>

#include "cthread_pool.h"

void* print_hello(void* args) {
    printf("%s\n", "hello");
    return 0;
}

void* calculate_square(void* args) {
    size_t x = (size_t)args;
    return (void*)(x * x);
}

int main() {
    thread_pool_t* p = create_thread_pool(4, 0);

    /**
     * its result can be treated ignorable because of 'NULL' in the 4th
     * parameter.
     */
    enqueue_task_thread_pool(p, print_hello, NULL, NULL);

    /**
     * its result can not be treated ignorable.
     */
    void* result = NULL;
    future_t* fut = enqueue_task_thread_pool(p, calculate_square, (void*)(size_t)100, &result);
    wait_and_close_future(fut);
    printf("100*100 = %d\n", (int)((size_t)result));

    close_thread_pool(p, true);
    return 0;
}