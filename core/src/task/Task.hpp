#pragma once

#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>

#include "../general/System.hpp"
#include "../general/Random.hpp"
#include "../general/Concurrent.hpp"

namespace task {

class Task {

protected:

	int id = effolkronium::random_static::get(0,999999);

	atomic<long long>	startExcute;
	atomic<long long>	nextExcute;

	long long			lastMeasure=0;
	int 				counter=0;

	string		taskName;

public:

	int				frequency=0;
	atomic<bool>	isStarted;
	atomic<bool>	isStopped;
	atomic<bool>	doStop;

	long iD = effolkronium::random_static::get(0, 1000000);

	bool isLongRunning() {
		return startExcute > 0 && System::currentTimeMillis() - startExcute > 10000;
	}

	Task() : Task(0,"") {};

	Task(int frequency, string taskName) :
			startExcute(0),
			nextExcute(System::currentTimeMillis() + frequency - 1),
			lastMeasure(System::currentTimeMillis()),
			taskName (taskName),
			frequency (frequency),
			isStarted(false),
			isStopped(false),
			doStop(false)
	{
		//cout << "task constructed " << id <<endl;
	}
	


	virtual ~Task() {
		cout << "task destructed " << id << " " << taskName << endl;
	};

	void start() {
		if (isStopped) {
			doStop =false;
			isStopped = false;
			isStarted = false;
			nextExcute = System::currentTimeMillis() + frequency - 1;
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
			this_thread::sleep_for(std::chrono::microseconds(wait));
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
		if (!doStop && isRunning() && (time - nextExcute) >= 0) {
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
	virtual void onStop() = 0;

}
;

class TaskExecute : public Task {
public: 
	void onStart() override {};
	void onStop() override {};
}
;

}

