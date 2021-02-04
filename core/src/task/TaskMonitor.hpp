#pragma once

#include <string>
#include <atomic>
#include <vector>
#include <chrono>
#include <thread>

#include <condition_variable>
#include "../general/Concurrent.hpp"
#include "../general/System.hpp"
#include "Task.hpp"

using namespace std;

class TaskMonitor {
	
private: 

	ConcurrentHashMap<long, shared_ptr<Task>> tasklist;
	ConcurrentQueue<Task*> executelist;
	atomic<long> sinceStarted;

	mutex mtx;
	condition_variable cv;

public:

	thread taskThread;
	string monitorName;

	long frequency = 1000; // 1 second default
	atomic<bool> isRunning;

	auto getRunningTasks() {
		return tasklist.values();
	}

	TaskMonitor(int frequency, string monitorName)
	: monitorName(monitorName),
	  frequency(frequency),
	  isRunning(false)
	{}

	~TaskMonitor() {
		for (shared_ptr<Task> task : tasklist.values()) {
			task->stop();
		}
		cv.notify_all();
		cout << "TaskMonitor destroyed" << endl;
	}

	void execute(Task* task) {
		shared_ptr<Task> task_s(task);
		tasklist.put(task_s->iD, task_s);
		task->start();
		addExcute(task);
	}
	void removeTask(Task* task) {
		tasklist.remove(task->iD);
	}

	// direct execute
	void addExcute(Task* task) {
		executelist.push(task);
		if (!isRunning.exchange(true)) {
			thread x (dorun, this);
			sinceStarted=System::currentTimeMillis();
		}
		cv.notify_all();
	}

	static void* dorun(void* This) {
		((TaskMonitor*)This)->run();
		return NULL;
	}

	void run() {
	
		try {

			vector<long> tasklistStopped;

			while (isRunning) {

				long long executiontime = System::currentTimeMillis(); // execution time
				// execute waiting tasks

				while (executelist.size() > 0) {
					Task* task = executelist.pop();
					// Execute Task
					if (task!=nullptr && !task->isStopped)
						task->execute(executiontime);
					//sinceStarted = System::currentTimeMillis();
				}
				for (shared_ptr<Task> task : tasklist.values()) {
					if (task->isStarted && task->isStopped) {
						tasklistStopped.push_back(task->iD);
					} else if (task->schedule(executiontime)) {
						executelist.push(task.get());
					} else if (task->doStop) {
						executelist.push(task.get());
					}
				}
				// check and reschedule tasks
				if (tasklistStopped.size() > 0) {
					for (long taskid : tasklistStopped) {
						tasklist.remove(taskid);
					}
					tasklistStopped.clear();
				}
				if (tasklist.size() == 0 && executelist.size() == 0) {
					isRunning=false;
				}
				// wait if execution list is empty
				if (isRunning && executelist.size() == 0) {
					// waiting to execute
					long waittime = (executiontime - System::currentTimeMillis() + frequency);
					if (waittime > 0 && executelist.size() == 0) {
						unique_lock<mutex> lk(mtx);
						cv.wait_for(lk, chrono::milliseconds(waittime));
					}
				}

				//cout << System::currentTimeMillis() - executiontime << endl;
			}
			executelist.clear();


		} catch (exception& ex) {
			cout << ex.what();
		}
		cout << "run ended" << endl;


	}
	
	void stopMonitor() {
		isRunning=false;
		cv.notify_all();
	}

}
;
