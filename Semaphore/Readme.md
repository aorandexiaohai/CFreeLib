# build environment

    method1:
    base_url="https://github.com/aorandexiaohai/" && wget $base_url/FormatHelper/raw/master/build_environment.py && python3 build_environment.py $base_url
    
    method2:
    base_url="https://gitee.com/xiatian_wzh/" && wget $base_url/FormatHelper/raw/master/build_environment.py && python3 build_environment.py $base_url

    gitee is a substitue of github in China.

# project description

    The project implements semaphore with pthread_mutex_t and pthread_cond_t.
    The related API has not changed.
    
    If you want want to use it, do the following:
        use '#include "csemaphore.h"' and never use '#include <semaphore.h>'
        define macro USE_DEFINED_SEMAPHORE
