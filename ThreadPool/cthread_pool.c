#include "cthread_pool.h"

#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#include "cdouble_list.h"

typedef struct task_inner task_t;
struct task_inner {
    generic_function_t f;
    generic_data_t args;
    generic_data_t result;
    sem_t *ready;
};
static task_t *create_task(generic_function_t f, generic_data_t args, generic_data_t result, sem_t *ready);
enum stop_command_t { STOP_INVALID = 0, STOP_IMMEDIATELY, STOP_UNTIL_ALL_FINISHED };
struct thread_pool_inner {
    int thread_count;
    pthread_t *threads;

    /**
     * properties that are related to the task queue
     */
    enum stop_command_t sc;
    int max_task_count;
    double_list_t *task_queue;
    pthread_mutex_t queue_lock;
    pthread_cond_t can_enqueue;
    pthread_cond_t can_dequeue;

    /**
     * properties that are related to the result
     */
    double_list_t *readies;
    pthread_mutex_t readies_lock;
};

static void free_semaphore(generic_data_t args) {
    sem_t *s = (sem_t *)args;
    sem_destroy(s);
    free(s);
}

struct future_inner {
    double_list_node_t *n;
    thread_pool_t *p;
};

static future_t *create_future(thread_pool_t *p, double_list_node_t *n);
static generic_data_t thread_call(generic_data_t args);

static bool is_pool_task_limited(thread_pool_t *p) { return p->max_task_count > 0; }

thread_pool_t *create_thread_pool(int thread_count, int max_task_count) {
    if (thread_count <= 0) {
        thread_count = 1;
    }

    thread_pool_t *p = (thread_pool_t *)calloc(sizeof(thread_pool_t), 1);
    p->thread_count = thread_count;
    p->threads = (pthread_t *)calloc(sizeof(pthread_t) * thread_count, 1);
    p->max_task_count = max_task_count;

    p->task_queue = create_double_list(free);
    pthread_mutex_init(&p->queue_lock, 0);

    pthread_cond_init(&p->can_dequeue, 0);

    if (is_pool_task_limited(p)) {
        pthread_cond_init(&p->can_enqueue, 0);
    }

    int i = 0;
    for (i = 0; i < thread_count; i++) {
        pthread_create(&p->threads[i], 0, thread_call, p);
    }

    p->readies = create_double_list(free_semaphore);
    pthread_mutex_init(&p->readies_lock, 0);

    return p;
}
future_t *enqueue_task_thread_pool(thread_pool_t *p, generic_function_t f, generic_data_t args, generic_data_t result) {
    sem_t *ready = NULL;
    double_list_node_t *node = NULL;
    if (result) {
        ready = (sem_t *)calloc(sizeof(sem_t), 1);
        sem_init(ready, 0, 0);

        pthread_mutex_lock(&p->readies_lock);
        node = push_back_double_list(p->readies, ready);
        pthread_mutex_unlock(&p->readies_lock);
    }

    task_t *task = create_task(f, args, result, ready);

    pthread_mutex_lock(&p->queue_lock);
    while (is_pool_task_limited(p) && p->task_queue->size == p->max_task_count) {
        pthread_cond_wait(&p->can_enqueue, &p->queue_lock);
    }
    push_back_double_list(p->task_queue, task);
    pthread_cond_signal(&p->can_dequeue);
    pthread_mutex_unlock(&p->queue_lock);

    return result ? create_future(p, node) : 0;
}
void wait_and_close_future(future_t *f) {
    if (f) {
        sem_wait((sem_t *)f->n->data);

        thread_pool_t *p = f->p;
        pthread_mutex_lock(&p->readies_lock);
        remove_double_list(p->readies, f->n);
        pthread_mutex_unlock(&p->readies_lock);
        free(f);
    }
}
void close_thread_pool(thread_pool_t *p, bool finished) {
    if (p) {
        pthread_mutex_lock(&p->queue_lock);
        if (!finished) {
            p->sc = STOP_IMMEDIATELY;
        } else {
            p->sc = STOP_UNTIL_ALL_FINISHED;
        }
        pthread_cond_broadcast(&p->can_dequeue);
        pthread_mutex_unlock(&p->queue_lock);

        int i = 0;
        for (i = 0; i < p->thread_count; i++) {
            pthread_join(p->threads[i], 0);
        }

        free_double_list(p->readies);
        free_double_list(p->task_queue);
        pthread_mutex_destroy(&p->readies_lock);
        pthread_mutex_destroy(&p->queue_lock);
        pthread_cond_destroy(&p->can_dequeue);

        if (is_pool_task_limited(p)) {
            pthread_cond_destroy(&p->can_enqueue);
        }
        free(p->threads);
        free(p);
    }
}
task_t *create_task(generic_function_t f, generic_data_t args, generic_data_t result, sem_t *ready) {
    task_t *task = (task_t *)calloc(sizeof(task_t), 1);
    task->f = f;
    task->args = args;
    task->result = result;
    task->ready = ready;
    return task;
}

future_t *create_future(thread_pool_t *p, double_list_node_t *n) {
    future_t *fut = calloc(sizeof(future_t), 1);
    fut->p = p;
    fut->n = n;
    return fut;
}
generic_data_t thread_call(generic_data_t args) {
    thread_pool_t *p = (thread_pool_t *)args;
    while (1) {
        pthread_mutex_lock(&p->queue_lock);
        while (p->task_queue->size == 0) {
            if (p->sc == STOP_IMMEDIATELY || p->sc == STOP_UNTIL_ALL_FINISHED) {
                break;
            }
            pthread_cond_wait(&p->can_dequeue, &p->queue_lock);
        }
        if (p->sc == STOP_IMMEDIATELY || (p->task_queue->size == 0 && p->sc == STOP_UNTIL_ALL_FINISHED)) {
            pthread_mutex_unlock(&p->queue_lock);
            break;
        }
        task_t *task = pop_front_double_list(p->task_queue);
        if (is_pool_task_limited(p)) {
            pthread_cond_signal(&p->can_enqueue);
        }
        pthread_mutex_unlock(&p->queue_lock);

        generic_data_t result = task->f(task->args);
        if (task->result) {
            *((generic_data_t *)task->result) = result;
            sem_post(task->ready);
        }
        free(task);
    }
    return NULL;
}