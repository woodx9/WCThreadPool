

#ifndef __BASICTASK__
#define __BASICTASK__ 
//任务运行的载体
class BasicTask {
public:
    BasicTask();

    //使用了设计模式：模板方法
    void run();

    virtual void task() = 0;

    bool status();

    virtual ~BasicTask();

//data
private:
    bool fstatus;
};

#endif // __BASICTASK__ 