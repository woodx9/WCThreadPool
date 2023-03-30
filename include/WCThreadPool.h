#include <iostream>
#include <vector>
#include <functional>
#include <unordered_set>
#include <mutex>
#include <utility>
#include <queue>
#include <condition_variable>
#include <thread>
#include "BasicTask.h"

#ifndef __WCTHREADPOOL__
#define __WCTHREADPOOL__ 
class WCThreadPool {
public:
     WCThreadPool(int );
     ~WCThreadPool();

     //将任务提交到这里
     void submit(BasicTask *);
//data
private:
    int thread_num, thread_pool_id;
    std::mutex mut;
    //因为要给condition_variable中参数，lamda函数中检测，所以要把任务队列设置成静态变量。
    //为了扩展的便捷性，所以把所有任务队列放在一个vector里面，线程池对象根据自己的thread_num找到对应的自己的任务队列。
    static std::vector<std::queue<BasicTask *> > tasksList;
    std::condition_variable cv;

//func
private:
    void worker();
};

#endif //__WCTHREADPOOL__ 