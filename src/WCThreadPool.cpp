#include "../include/WCThreadPool.h"
using namespace std;




WCThreadPool::WCThreadPool(int t_thread_num): thread_num(t_thread_num){
    thread_pool_id = tasksList.size();
    tasksList.push_back(queue<BasicTask *>() );
    
    for(int i = 0; i < thread_num; ++i) {
        //thread构造函数第一个参数必须定义为全局函数或者类里的静态函数，不过采用std::bind()函数可以直接绑定类成员函数
        thread t(std::bind(&WCThreadPool::worker, this));
        t.detach();
    }
}

WCThreadPool::~WCThreadPool() {
    
}

void WCThreadPool::submit(BasicTask * bt) {
    lock_guard<mutex> lk(mut);
    tasksList[thread_pool_id].push(bt);
    cv.notify_one();
}





void WCThreadPool::worker() {
    while (true) {
        unique_lock<mutex> lk(mut);
        bool t_thread_pool_id = thread_pool_id;
        cv.wait(lk, [t_thread_pool_id](){return !WCThreadPool::tasksList[t_thread_pool_id].empty(); });

        BasicTask * bt = tasksList[thread_pool_id].front();
        tasksList[thread_pool_id].pop();

        lk.unlock();

        //工作中
        (*bt).run();
    }
}


//初始化静态变量
std::vector<std::queue<BasicTask*> > WCThreadPool::tasksList;



