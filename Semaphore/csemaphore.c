#include "csemaphore.h"
#if defined(USE_DEFINED_SEMAPHORE)
#include <pthread.h>
#include <stdlib.h>

struct sem_inner {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int value;
};

int sem_init(sem_t *sem, int pshared, unsigned int value) {
    sem->value = value;

    pthread_mutex_init(&(sem->lock), 0);
    pthread_cond_init(&(sem->cond), 0);

    return 0;
}

int sem_post(sem_t *sem) {
    if (sem) {
        pthread_mutex_lock(&sem->lock);
        sem->value++;
        if (sem->value > 0) {
            pthread_cond_signal(&sem->cond);
        }
        pthread_mutex_unlock(&sem->lock);
    }
    return 0;
}

int sem_wait(sem_t *sem) {
    if (sem) {
        pthread_mutex_lock(&sem->lock);
        while (sem->value <= 0) {
            pthread_cond_wait(&sem->cond, &sem->lock);
        }
        sem->value--;
        pthread_mutex_unlock(&sem->lock);
    }
    return 0;
}

int sem_destroy(sem_t *sem) {
    if (sem) {
        pthread_cond_destroy(&sem->cond);
        pthread_mutex_destroy(&sem->lock);
    }
    return 0;
}
int sem_getvalue(sem_t *sem, int *sval) {
    pthread_mutex_lock(&sem->lock);
    *sval = sem->value;
    pthread_mutex_unlock(&sem->lock);
    return 0;
}
#endif