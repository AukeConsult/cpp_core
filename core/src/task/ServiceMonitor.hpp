#pragma once

#include <string>
#include "TaskMonitor.hpp"

namespace task {

class ServiceMonitor {

	string name;

public:

	TaskMonitor* pingMonitor;
	ServiceMonitor(string name) : name(name) {
		start();
	}
	~ServiceMonitor() {
		stop();
	}
	void start() {
		pingMonitor = new TaskMonitor(1000, "pingMonitor " + name);
	}
	void stop() {
		pingMonitor->stopMonitor();
		delete pingMonitor;
	}

};

}
