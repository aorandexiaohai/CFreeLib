#ifndef CSEMAPHORE_H
#define CSEMAPHORE_H
/**
 * #define USE_DEFINED_SEMAPHORE
 */
#if defined(USE_DEFINED_SEMAPHORE)
/**
 * The defined semaphore is implemented by pthread_mutex_t and pthread_cond_t.
 * So the function of the defined semaphore will limited by pthread_mutex_t and pthread_cond_t.
 */
typedef struct sem_inner sem_t;

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_destroy(sem_t *sem);
int sem_getvalue(sem_t *sem, int *sval);

#else
#include <semaphore.h>
#endif
#endif