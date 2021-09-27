# description

    The project implements semaphore with pthread_mutex_t and pthread_cond_t.
    The related API has not changed.
    
    If you want want to use it, do the following:
        use '#include "csemaphore.h"' and never use '#include <semaphore.h>'
        define macro USE_DEFINED_SEMAPHORE
