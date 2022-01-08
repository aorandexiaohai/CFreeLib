![](https://gitee.com/wuzhenhai1/my-cdn/raw/master/ThreadPoolDesign.png)

# C语言轻型线程池

    该线程池特性:
        1) 任务通用性
            支持的函数原型为: void* any_function(void*).

        2) 支持设置线程数

        3) 支持设置最大任务数限制(默认无限制). 
            如果当前任务缓存已满, 则'enqueue_task'调用将被阻塞

        4) 支持线程池计算结果异步反馈
            如果'enqueue_task'中的'result'指针不为空, 该函数返回一个future_t支持.
            调用者通过调用'wait_and_close_future'等待线程池的结果. 
            最后通过'result'指针获取结果.
            
            如果'result'指针不为空, 调用者不能忘记调用'wait_and_close_future', 否则会有
            资源泄露.

            如果'result'指针为空, 'enqueue_task'也将返回空指针. 这意味着用户不关心计算结果.

            'result'指向的内存必须至少能够容纳sizeof(void*)个字节.
        
        5)  支持手动关闭线程池. 关闭选项包括是否等待完成所有任务.

# 任务池组件源码获取

    限制: 
        只适用于主流linux系统, 比如redhat, centos, ubuntu等. 
        系统安装了git, python3等.

    获取命令:
        git clone git@github.com:aorandexiaohai/CFreeLib.git
        cd CFreeLib
        cd ThreadPool && cp ../build_environment.py . && python3 build_environment.py
    
    最后, 当前目录下(非递归)的所有.c, .h文件就是ThreadPool的完整代码.

# 使用例子

[import](https://raw.githubusercontent.com/aorandexiaohai/CFreeLib/master/ThreadPool/examples/example.c)
