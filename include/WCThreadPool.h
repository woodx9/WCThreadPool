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
     WCThreadPool(int, int);
     WCThreadPool(const WCThreadPool &) = delete;
     WCThreadPool & operator=(const WCThreadPool &) = delete;
     ~WCThreadPool();

     //将任务提交到这里
     //提交成功返回true，提交失败返回false
     //int参数表示优先级
     //task queue在tasksList中的下标越小，代表优先级越高
     bool submit(BasicTask *, int);
//data
private:
    int thread_num, task_queue_num, all_task_num;
    //当没有任务时需要, 悬挂线程, 需要用它们检查all_task_num是否为0
    std::mutex idle_mutex;
    std::condition_variable idle_cv;

    std::vector<std::mutex *> mutexsList;
    //task queue在tasksList中的下标越小，代表优先级越高
    std::vector<std::queue<BasicTask *> > tasksList;
    std::vector<std::condition_variable> cvList;

//func
private:
    void worker();
};

#endif //__WCTHREADPOOL__ 