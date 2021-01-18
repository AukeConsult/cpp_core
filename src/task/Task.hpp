#pragma once

#include <iostream>
#include <atomic>
#include <thread>

#include "../general/System.hpp"
#include "../general/Random.hpp"
#include "../general/Concurrent.hpp"

class Task {

protected:

	atomic<long>	nextExcute = 0;
	atomic<long>	startExcute = 0;

	// watch frequency
	long lastMeasure=0;
	int counter=0;

	string			taskName;

public:

	int				frequency = 0;
	atomic_bool		isStarted = false;
	atomic<bool>	isStopped = false;
	atomic<bool>	doStop = false;

	long iD = effolkronium::random_static::get(0L, LONG_MAX);

	bool isLongRunning() {
		return startExcute > 0 && System::currentTimeMillis() - startExcute > 10000;
	}

	Task(int frequency_) {
		frequency = frequency_;
		isStopped=true;
		isStarted=false;
		nextExcute = System::currentTimeMillis() + frequency - 1;
		lastMeasure = System::currentTimeMillis();
	
	}
	Task(string taskName_, int frequency_) : Task(frequency_) {
		taskName = taskName_;
	}

	void start() {
		if (isStopped) {
			doStop=false;
			isStopped=false;
			isStarted=false;
			nextExcute= System::currentTimeMillis() + frequency - 1;
		}
	
	}
	void stop() {
		if (!isStopped) {
			doStop=true;
		}
	}
	// force stopping
	// don't use monitor execution to fire stop
	void forceStop() {
		if (!isStopped.exchange(true)) {
			onStop();
		}
	}
	bool isCompleted() {
		return isStarted && isStopped;
	}
	void waitStopped() {
		int wait = 0;
		while (wait < frequency && doStop) {
			std::this_thread::sleep_for(std::chrono::microseconds(wait));
			wait += 10;
		}
	}
	bool isRunning() {
		return isStarted && !isStopped;
	}
	void waitFor(long time) {
		nextExcute= System::currentTimeMillis() + time;
	}
	bool schedule(long time) {
		if ((time - nextExcute) >= 0) {
			return true;
		}
		return false;
	}
	void execute(long time) {
		if ((System::currentTimeMillis() - lastMeasure) > frequency * 10) {
			if (counter > 20) {
				cout << taskName << " taskid " << iD << " to many executions " << counter << endl;
			}
			counter = 0;
		}
		counter++;
		startExcute= System::currentTimeMillis();
		if (!isStarted.exchange(true)) {
			onStart();
		}

		if (doStop) {
			// do stop execution
			if (!isStopped.exchange(true)) {
				onStop();
			}
		}

		if (isStarted && !isStopped) {
			// set frequency before execute in case next execute time is set
			// inside on execute
			// make a test for this
			// calculate next execution time
			nextExcute= time + frequency - 1;
			// to make sure stop signal get at close to execute as possible
			if (!isStopped) {
				onExecute();
			}
		}
		startExcute=0;
	}

	virtual void onStart() = 0;
	virtual void onExecute() = 0;
	virtual void onStop()  = 0;

}
;

class TaskExecute : public Task {
public: 
	TaskExecute(int frequency_) : Task(frequency_) {}
	TaskExecute(string taskName, int frequency_) : Task(taskName, frequency_) {}
	void onStart() {};
	void onStop() {};
}
;
