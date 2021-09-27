# build environment
    
    method1:
    
    base_url="https://github.com/aorandexiaohai/" && wget $base_url/FormatHelper/raw/master/build_environment.py && python3 build_environment.py $base_url
    
    method2:
    base_url="https://gitee.com/xiatian_wzh/" && wget $base_url/FormatHelper/raw/master/build_environment.py && python3 build_environment.py $base_url


    gitee is a substitue of github in China.
    
# project description

    The project provides an implementation of a thread pool by C language on the  most linux OS.
    
    Its features:
        1) Clean and commented header file. There are only 4 APIs in total for public usage.
        2) The result can be fetched or ignored.

# pattern of API calling workflow.

    see the details in ./examples/test1.c

    gcc -Wall cthread_pool.c cthread_pool.h cdouble_list.c  cdouble_list.h csemaphore.h csemaphore.c ./examples/test1.c -I. -lpthread -O3 -o test1




