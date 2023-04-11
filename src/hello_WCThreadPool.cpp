#include "../include/WCThreadPool.h"
#include "../include/BasicTask.h"
using namespace std;

class MyTask : public BasicTask {
public:
    MyTask(int t_num) : num(t_num) {
        result = -1;
    }

    //在这里写需要实现的功能
    void task() override{
        result = num + 1;   
    }

    //拿出结果
    int get_result() {
        return result;   
    }

    virtual ~MyTask() {

    }

//在这里还可以添加很多数据
private:
    int num;
    int result;
};


int main() {
    WCThreadPool tp(1, 10);
    int i = 0;
    vector<BasicTask *> vb;
    while (i < 20) {
        BasicTask * bt = new MyTask(i++);
        tp.submit(bt, 0);
        vb.push_back(bt);
    }

    

    for (int i = 0; i < 20; ++i) {
        MyTask * mt = (MyTask *)vb[i];
        while (!mt->status()) ;
            cout << mt->get_result() << endl;
    }

    while (1) ;
    return 0;
}

