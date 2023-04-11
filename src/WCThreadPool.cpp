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
    
}

bool WCThreadPool::submit(BasicTask * bt, int priority) {
    if (priority < 0 || priority >= task_queue_num) return false;

    lock_guard<mutex> lk(*mutexsList[priority]);
    tasksList[priority].push(bt);
    cvList[priority].notify_one();

    //增加任务数量
    lock_guard<mutex> idle_lk(idle_mutex);
    all_task_num += 1;
    return true;
}


void WCThreadPool::worker() {
    while (true) {
            // int i = 0;
        for (int i = 0; i < task_queue_num; ++i) {
            if (!tasksList[i].empty()) {
                unique_lock<mutex> lk(*mutexsList[i]);

                cvList[i].wait(lk, [this, i](){return !tasksList[i].empty(); });

                BasicTask * bt = tasksList[i].front();
                tasksList[i].pop();

                lk.unlock();

                //工作中
                (*bt).run();

                //重置遍历，从i = 0开始看有没有优先级高的任务需要执行
                i = -1;
                //减少任务数量
                lock_guard<mutex> idle_lk(idle_mutex);
                all_task_num -= 1;
            }
        }

        unique_lock<mutex> idle_lk(idle_mutex);
        idle_cv.wait(idle_lk, [this](){return all_task_num > 0;});
    }
}




