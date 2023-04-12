#include "../include/WCThreadPool.h"
using namespace std;

WCThreadPool::WCThreadPool(int t_thread_num, int t_task_queue_num): thread_num(t_thread_num), task_queue_num(t_task_queue_num), tasksList(t_task_queue_num), cvList(t_task_queue_num){
    all_task_num = 0;
    
    for (int i = 0; i < t_task_queue_num; ++i) {
        mutexsList.push_back(new mutex());
    }
    
    for(int i = 0; i < thread_num; ++i) {
        thread t(std::bind(&WCThreadPool::worker, this));
        t.detach();
    }
}

WCThreadPool::~WCThreadPool() {
    //删除所有mutex
    for (int i = 0; i < task_queue_num; ++i) {
        delete mutexsList[i];
    }
}

bool WCThreadPool::submit(BasicTask * bt, int priority) {
    if (priority < 0 || priority >= task_queue_num) return false;

    lock_guard<mutex> lk(*mutexsList[priority]);
    tasksList[priority].push(bt);

    //增加任务数量
    lock_guard<mutex> idle_lk(idle_mutex);
    all_task_num += 1;
    idle_cv.notify_one();
    return true;
}


void WCThreadPool::worker() {
    while (true) {
        unique_lock<mutex> idle_lk(idle_mutex);
        idle_cv.wait(idle_lk, [this](){return all_task_num > 0;});
        idle_lk.unlock();
        for (int i = 0; i < task_queue_num; ++i) {
            unique_lock<mutex> lk(*mutexsList[i]);
            if (!tasksList[i].empty()) {

                BasicTask * bt = tasksList[i].front();
                tasksList[i].pop();

                lk.unlock();
                //执行线程需要的任务
                (*bt).run();

                //重置遍历，从i = 0开始看有没有优先级高的任务需要执行
                i = -1;
                
                //减少任务数量
                lock_guard<mutex> idle_lk_inside(idle_mutex);
                all_task_num -= 1;
            }
        }
    }
}




