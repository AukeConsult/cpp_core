#pragma once

#include <thread>

using namespace std;

class Runnable {
public:
    virtual void run() = 0;
}
;

class ExecutorService {
public:
    thread execute(Runnable* task) {
        thread t([task] {
            task->run();
            }
        );
        return t;
    }
};