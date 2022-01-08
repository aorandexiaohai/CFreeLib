#ifndef CTHREAD_POOL_H
#define CTHREAD_POOL_H
#include "cbase.h"

typedef struct thread_pool_inner thread_pool_t;

/**
 * A future result type returned by 'enqueue_task_thread_pool'.
 */
typedef struct future_inner future_t;

/**
 * A function pointer provided by the user of the pool when using
 * 'enqueue_task_thread_pool'.
 */

/**
 * @pre 'threads' is positive.
 *
 * @param threads The thread count of used threads in the pool.
 * @param max_task_count The count limit of all tasks in the pool.
 *      If it is not positive, there is no limit.
 *      Otherwise, 'enqueue_task_thread_pool' will be pending until there is an empty slot
 * in the queue.
 *      While 'max_task_count' is not an important variable, you can set it zero
 * almostly.
 *
 */
thread_pool_t *create_thread_pool(int threads, int max_task_count);

/**
 * @pre 'p' is not NULL.
 * @pre The user of the pool should maintain the life time of 'args' and
 * 'result' if necessary.
 * @pre If result is not NULL, it can hold at least sizeof(void*) of bytes. This
 * is very important!!! The reason is that it is used to store the returned result 
 * of 'f'.
 *
 * @param p A thread pool.
 * @param f
 * @param args
 * @param result Memory address that is used to store the returned value of 'f'.
 *
 * @return A future pointer.
 *      If 'result' or 'f' is NULL, the function will reture NULL.
 */
future_t *enqueue_task_thread_pool(thread_pool_t *p, generic_function_t f, generic_data_t args, generic_data_t result);

/**
 * @pre 'wait_and_close_future' must be called only once for a non-null future
 * object.
 *
 * @brief If the user wants to fetch the correct content in the 'result' address
 * or wait for the finishment of his/her function in the pool,
 * 'wait_and_close_future' should be called firstly.
 *
 * @param f A future pointer returned by 'enqueue_task_thread_pool'.
 */
void wait_and_close_future(future_t *f);

/**
 * @pre 'p' is not NULL.
 * @brief Before calling 'close_thread_pool', any pending calls that are related
 * the pool should not exist.
 *
 * @param p A thread pool.
 * @param finished
 *      If it is true, it will return until all existing tasks have been
 * finished. Otherwise, it will try to stop immediately.
 */
void close_thread_pool(thread_pool_t *p, bool finished);

#endif