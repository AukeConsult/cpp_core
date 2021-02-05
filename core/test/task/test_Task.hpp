
/*
 * test_task.hpp
 *
 *  Created on: Jan 26, 2021
 *      Author: leif
 */

#pragma once

#include <thread>
#include <memory>
#include "gtest/gtest.h"
#include "../../src/task/Task.hpp"
#include "../../src/task/TaskMonitor.hpp"

namespace task {

class TaskTest: public Task {

private:
	int cnt=0;
public:

	TaskTest() : Task() {};
	TaskTest(int frequency, string taskName) : Task(frequency, taskName) {};
	void onStart() {
		cout << "start " << id << endl;
	};
	void onExecute() {
		cnt++;
		this_thread::sleep_for(10ms);
		cout << id << taskName << endl;
	};
	void onStop() {
		cout << "stop " << id << " " << taskName << " cnt " << cnt << endl;
	}
};

TEST(TaskTest, initTask) {
	shared_ptr<TaskTest> task = make_shared<TaskTest>();
}

TEST(TaskTest, initmonitor) {

	TaskMonitor* pingMonitor(new TaskMonitor(100, "pingMonitor"));

	TaskTest* task1 = new TaskTest(1000,"task1");
	TaskTest* task2 = new TaskTest(500,"task2");
	TaskTest* task3 = new TaskTest(2000,"task3");
	TaskTest* task4 = new TaskTest(250,"task4");

	pingMonitor->execute(task1);
	pingMonitor->execute(task2);
	pingMonitor->execute(task3);
	pingMonitor->execute(task4);

	this_thread::sleep_for(5s);

	delete pingMonitor;

	cout << pingMonitor->monitorName << " end" << endl;

}

TEST(TaskTest, startStopp) {

	shared_ptr<TaskMonitor> pingMonitor(new TaskMonitor(100, "pingMonitor"));
	cout << pingMonitor->monitorName << endl;

	TaskTest* task1 = new TaskTest(1000,"task1");
	TaskTest* task2 = new TaskTest(500,"task2");
	TaskTest* task3 = new TaskTest(2000,"task3");
	TaskTest* task4 = new TaskTest(250,"task4");

	pingMonitor->execute(task1);
	pingMonitor->execute(task2);
	pingMonitor->execute(task3);
	pingMonitor->execute(task4);

	long long start = System::currentTimeMillis();
	while((System::currentTimeMillis() - start) < 5000) {
		cout << "wait... " << System::currentTimeMillis() - start << endl;
		this_thread::sleep_for(3s);
		task3->stop();
	}

	cout << pingMonitor->monitorName << " end" << endl;

}

}

