#pragma once

#include <thread>

using namespace std;

class Runnable {
public:
    virtual void run() const = 0;
    virtual ~Runnable() {};
}
;

class ExecutorService {
public:
    virtual ~ExecutorService() {};
    thread execute(Runnable* task) {
        thread t([task] {
            task->run();
            }
        );
        return t;
    }
};
