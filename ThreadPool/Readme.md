  # description

    The project provides an implementation of a thread pool by C language on the  most linux OS.
    
    Its features:
        1) Clean and commented header file. There are only 4 APIs in total for public usage.
        2) The result can be fetched or ignored.

# pattern of API calling workflow.

    see the details in ./examples/test1.c

    gcc -Wall cthread_pool.c cthread_pool.h cdouble_list.c  cdouble_list.h csemaphore.h csemaphore.c ./examples/test1.c -I. -lpthread -O3 -o test1




