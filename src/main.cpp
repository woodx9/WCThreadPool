#include "../include/WCThreadPool.h"
#include "../include/BasicTask.h"
using namespace std;

class B : public BasicTask {
public:
    B(int t_num) : num(t_num) {
        result = -1;
    }

    //在这里写需要实现的功能
    virtual void task() {
        result = num + 1;   
    }

    //拿出结果
    int get_result() {
        return result;   
    }

    virtual ~B() {

    }

//在这里还可以添加很多数据
private:
    int num;
    int result;
};


int main() {
    WCThreadPool tp(10);
    
    int i = 0;
    vector<BasicTask *> vb;
    while (i < 20) {
        BasicTask * bt = new B(i++);
        tp.submit(bt);
        vb.push_back(bt);
    }

    for (int i = 0; i < 20; ++i) {
        B * b = (B *)vb[i];
        cout << (b)->get_result() << endl;
    }

    while (1) ;
    return 0;
}

