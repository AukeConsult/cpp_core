#pragma once

#include <string>
#include <atomic>
#include <vector>
#include <chrono>
#include "../general/Executor.hpp"
#include "../general/Concurrent.hpp"
#include "../general/System.hpp"
#include "Task.hpp"

using namespace std;

class TaskMonitor : Runnable {
	
private: 

	ExecutorService* executor;
	ConcurrentHashMap<long, shared_ptr<Task>> tasklist;
	ConcurrentQueue<shared_ptr<Task>> executelist;
	atomic<long> sinceStarted = 0;

	mutex mtx;
	condition_variable cv;

public:

	string monitorName;

	long frequency = 1000; // 1 second default
	atomic<bool> isRunning = true;

	vector<shared_ptr<Task>> getRunningTasks() {
		return tasklist.values();
	}

	TaskMonitor(ExecutorService* executor_, int frequency_, string monitorName_) {
		if (frequency_ > 0) frequency = frequency_;
		executor = executor_;
		monitorName = monitorName_;
		isRunning=false;
	}
	void execute(Task* task) {
		tasklist.put(task->iD, make_shared<Task>(task));
		task->start();
		addExcute(task);
	}
	void removeTask(Task* task) {
		tasklist.remove(task->iD);
	}
	// direct execute
	void addExcute(Task* task) {	
		if (task != nullptr) {
			executelist.push(make_shared<Task>(task));
			if (!isRunning.exchange(true)) {
				executor->execute(this);
				sinceStarted= System::currentTimeMillis();
			}
			cv.notify_all();
		}
	}

	void run() {
	
		vector<long> tasklistStopped;
		
		while (isRunning) {
			
			long executiontime = System::currentTimeMillis(); // execution time
			// execute waiting tasks
			
			while (executelist.size() > 0) {
				shared_ptr<Task> task = executelist.pop();
				// Execute Task
				if (task != nullptr && !task->isStopped)
					task->execute(executiontime);
				sinceStarted = System::currentTimeMillis();
			}

			// check and reschedule tasks
			for (shared_ptr<Task> task : tasklist.values()) {
				if (task->isStarted && task->isStopped) {
					tasklistStopped.push_back(task->iD);
				}
				else if (task->schedule(executiontime)) {
					executelist.add(make_shared<Task>(task));
				}
			}
			if (tasklistStopped.size() > 0) {
				for (long taskid : tasklistStopped) {
					tasklist.remove(taskid);
				}
				tasklistStopped.clear();
			}
			if (tasklist.size() == 0 && executelist.size() == 0) {
				if (System::currentTimeMillis() - sinceStarted > (frequency * 2)) {
					isRunning=false;
				}
			}
			// wait if execution list is empty
			if (isRunning && executelist.size() == 0) {
				// waiting to execute
				long waittime = (executiontime - System::currentTimeMillis() + frequency);
				if (waittime > 0 && executelist.size() == 0) {
					unique_lock<mutex> lk(mtx);
					cv.wait_for(lk, waittime *1ms);
				}
			}
		}
		executelist.clear();
	}
	
	void stopMonitor() {
		isRunning=false;
		cv.notify_all();
	}

}
;