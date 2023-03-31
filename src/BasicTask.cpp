#include "../include/BasicTask.h"

BasicTask::BasicTask() {
    fstatus = false;
}

//使用了模板方法
void BasicTask::run() {
    task();
    fstatus = true;
}

bool BasicTask::status() {
    return fstatus;
}

BasicTask::~BasicTask() {

}
