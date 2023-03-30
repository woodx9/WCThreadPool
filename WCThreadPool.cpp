#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <unordered_map>
#include <set>
#include <stack>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <utility>
#include <queue>
#include <condition_variable>
#include <thread>
#include <functional>
using namespace std;

//多线程
//函数对象
class A {
public:
    A(int t_num) : num(t_num) {
        status = false;
        result = -1;
    }

    virtual void run() {
        cout << num << endl;
        status = true;
        result = num + 1;
    }

    bool get_status() {
        return status;
    }

    int get_result() {
        return result;   
    }

    virtual ~A() {

    }
private:
    int num;
    bool status;
    int result;
};





class WCThreadPool {
public:
    WCThreadPool(int t_thread_num): thread_num(t_thread_num){
        for(int i = 0; i < thread_num; ++i) {
            thread t(std::bind(&WCThreadPool::worker, this));
            t.detach();
        }
        queue<A *> temp_q;
        q.push_back(std::move(temp_q));
    }

    void submit(A* a) {
        lock_guard<mutex> lk(mut);
        q[0].push(a);
        cv.notify_one();
    }

    
//data
private:
    int thread_num;
    mutex mut;
    static vector<queue<A*> > q;
    int queue_num;
    condition_variable cv;

//func
private:

    //work_thread
    //必须定义为全局函数或者类里的静态函数
    void worker() {
        while (1) {
            unique_lock<mutex> lk(mut);
            bool empty = q[0].empty();
            cv.wait(lk, [](){return !WCThreadPool::q[0].empty(); });

            A * a = q[0].front();
            q[0].pop();

            lk.unlock();

            //工作中
            (*a).run();
        }
    }


};

//初始化静态变量
vector<queue<A*> > WCThreadPool::q;

int main() {
    WCThreadPool tp(10);
    
    int w;
    cin >> w;

    int i = 0;
    vector<A *> va;
    while (i < 100) {
        A * a = new A(i++);
        tp.submit(a);
        va.push_back(a);
    }

    
    cin >> w;

    for (int i = 0; i < 100; ++i) {
        // while (!va[i]->get_status()) ;
            cout << (va[i])->get_result() << endl;
    }

    while (1) ;
    return 0;
}

