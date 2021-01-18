#pragma once

#include <string>
#include "../general/Executor.hpp"
#include "TaskMonitor.hpp"

class ServiceMonitor {

	string name;
	ExecutorService threadpool;

public:

	TaskMonitor* pingMonitor;
	
	ServiceMonitor(string name_) {
		name = name_;
		start();
	}
	~ServiceMonitor() {
		stop();
	}

	void start() {
		pingMonitor = new TaskMonitor(&threadpool, 1000, "pingMonitor " + name);
	}

	void stop() {
		pingMonitor->stopMonitor();
		delete pingMonitor;
	}

};
