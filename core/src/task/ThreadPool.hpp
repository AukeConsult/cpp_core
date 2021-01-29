#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include <deque>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <condition_variable>

const int MAX_POOL_SIZE = 10;
const int MIN_POOL_SIZE = 2;
const int TREAD_LIVE=2000;
const int WAIT_TIME=1000;

using namespace std;
using namespace std::chrono;

class ThreadPool;

class ThreadPoolTask {
public:
	ThreadPoolTask(void (*fn_ptr)(void*), void* arg) : m_fn_ptr(fn_ptr), m_arg(arg) {}
	~ThreadPoolTask() {}
	void operator()() {
		(*m_fn_ptr)(m_arg);
//		if (m_arg != NULL) {
//			delete m_arg;
//		}
	}
	void run() {
		(*m_fn_ptr)(m_arg);
	}
private:
	void (*m_fn_ptr)(void*);
	void* m_arg;
};

class ThreadWorker {
public:

	thread			self;
	ThreadPool * 	pool=nullptr;
	ThreadWorker() :
			executing(false),
			running(true),
			m_last_execute(0)
	{}
	~ThreadWorker() {};
	void* execute_thread(ThreadWorker * worker);
	atomic<bool> 	executing, running;

private:
	long long 		m_last_execute;
	long long now() {
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}
};


extern "C" void* start_thread(void* This){
	ThreadWorker* tw = (ThreadWorker*) This;
	tw->execute_thread(tw);
	return NULL;
}

class ThreadPool {
public:

	atomic<bool> stopped;

	ThreadPool() : stopped(false), m_pool_size(MAX_POOL_SIZE) {
		cout << "Constructed ThreadPool of size " << m_pool_size << endl;
	}
	ThreadPool(int pool_size) : stopped(false), m_pool_size(pool_size) {
		cout << "Constructed ThreadPool of size " << m_pool_size << endl;
	}
	~ThreadPool() {
		cout << "signal to all threads..." << endl;
		stopped = true;
		m_wait_cond.notify_all();
		for (auto tr : m_threads) {
		    tr->self.join();
			m_wait_cond.notify_all();
		}
		cout << "threads exited from the thread pool" << endl;
	}

	void initialize_thread() {
		try {

			ThreadWorker* worker = new ThreadWorker();
			m_threads.push_back(worker);

			worker->pool=this;
			worker->self = thread(start_thread, worker);
			//cout << "Thread started" << worker->self.get_id() << endl;

		} catch (exception& ex) {
			cerr << ex.what() << endl;
		}
	}

	bool canStop () {
		unique_lock<mutex> lock(m_lock);
        m_threads.erase(
        		remove_if(m_threads.begin(),
        				m_threads.end(),
						[](const ThreadWorker* tw) { return !tw->running;}
        		)
				, m_threads.end()
		);
        return m_threads.size()>MIN_POOL_SIZE;
	}

	void wait() {
        unique_lock<mutex> lock(m_lock);
		while (!stopped && (m_tasks.empty())) {
		    m_wait_cond.wait(lock);
		}
	}

	void wait_for(int time) {
        unique_lock<mutex> lock(m_lock);
		if (!stopped) {
		    m_wait_cond.wait_for(lock,time*1ms);
		}
	}

	bool hasTasks () {
        unique_lock<mutex> lock(m_lock);
		bool ret = !m_tasks.empty() && !stopped;
		return ret;
	}

	ThreadPoolTask* popTask() {
        unique_lock<mutex> lock(m_lock);
		if(m_tasks.size()>0) {
			ThreadPoolTask* task = m_tasks.front();
			m_tasks.pop_front();
			return task;
		}
		return nullptr;
	}

	void add_task(ThreadPoolTask* task) {
        unique_lock<mutex> lock(m_lock);
        m_threads.erase(
        		remove_if(m_threads.begin(),
        				m_threads.end(),
						[](const ThreadWorker* tw) { return !tw->running;}
        		)
				, m_threads.end()
		);
        for (int i = m_threads.size(); i < 2; i++) {
			initialize_thread();
		}

        if(m_threads.size()<m_pool_size) {
        	auto it = find_if(
        			m_threads.begin(),
        			m_threads.end(),
					[](const ThreadWorker* tw) { return !tw->executing; }
        	);

        	if(it !=  m_threads.end()) {
        		initialize_thread();
        		cerr << "make threadx, num " << m_threads.size() << endl;
        	}
        }
		m_tasks.push_back(task);
		m_wait_cond.notify_one();
	}

private:

	size_t m_pool_size;
	mutex m_lock;
	condition_variable m_wait_cond;
	vector<ThreadWorker*> m_threads; // storage for threads
	deque<ThreadPoolTask*> m_tasks;

};

void* ThreadWorker::execute_thread(ThreadWorker* tw) {

	try {

		while(!pool->stopped&&running) {

			// Try to pick a task
			// If the thread was woken up to notify process shutdown, return from here

			if (pool->hasTasks()) {

				ThreadPoolTask* task = pool->popTask();
				if(task!=nullptr) {
					// execute the task
					executing=true;
					m_last_execute = now();
					(*task)(); // could also do task->run(arg);
					delete task;
					m_last_execute = now();
					executing=false;
				}

			} else {
				// check for how long without running
				if(pool->canStop() && (now() - m_last_execute > TREAD_LIVE)) {
					running=false;
				}
			}

			if (!pool->stopped&&running)
				pool->wait_for(WAIT_TIME);

		}
		cerr << "thread end " << tw->self.get_id() << endl;

	} catch(exception& ex) {
		cerr << ex.what() << tw->self.get_id() << endl;
		running=false;
		executing=false;
	}
	return NULL;
}
