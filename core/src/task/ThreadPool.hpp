#pragma once

#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <iostream>
#include <vector>
#include <errno.h>
#include <string>

#include "CondVar_Mutex.hpp"

#include <atomic>
//#include <thread>


const int DEFAULT_POOL_SIZE = 10;
const int STARTED = 0;
const int STOPPED = 1;

using namespace std;

class ThreadPool;

class ThreadWork {

public:

	pthread_t 		self=0;
	ThreadPool * 	pool=nullptr;
	atomic<bool> 	executing, started, stopped;

	ThreadWork() :
			executing(false),
			started(false),
			stopped(false)
			{};

	~ThreadWork() {};

	void* execute_thread();

private:

};

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


extern "C" void* start_thread(void* This){
//		ThreadPool* tp = (ThreadPool*) This;
//		tp->execute_thread();

	ThreadWork* tp = (ThreadWork*) This;
	tp->execute_thread();
	return NULL;
}

class ThreadPool {
public:

	atomic<bool> stopped;
	ThreadPool() : m_pool_size(DEFAULT_POOL_SIZE) {
		cout << "Constructed ThreadPool of size " << m_pool_size << endl;
	}
	ThreadPool(int pool_size) : m_pool_size(pool_size) {
		cout << "Constructed ThreadPool of size " << m_pool_size << endl;
	}
	~ThreadPool() {
		if (!stopped) {
			destroy_threadpool();
		}
	}
	int initialize_threadpool() {
		// TODO: COnsider lazy loading threads instead of creating all at once
		stopped=true;
		for (int i = 0; i < m_pool_size; i++) {
			//pthread_t tid;
			ThreadWork worker;
			worker.pool=this;
			int ret = pthread_create(&worker.self, NULL, start_thread, (void*) &worker);
			if (ret != 0) {
				cerr << "pthread_create() failed: " << ret << endl;
				return -1;
			}
			cout << "Thread started: " << worker.self << endl;
			m_threads.push_back(worker);
		}
		//cout << m_pool_size << " threads created by the thread pool" << endl;
		return 0;
	}

	void lock() {m_task_mutex.lock();}
	void unlock() {m_task_mutex.unlock();}
	void wait() {
		m_task_mutex.lock();
		while (!stopped && (m_tasks.empty())) {
			m_task_cond_var.wait(m_task_mutex.get_mutex_ptr());
		}
		m_task_mutex.unlock();
	}

	ThreadPoolTask* popTask() {
		m_task_mutex.lock();
		ThreadPoolTask* task;
		if(m_tasks.size()>0) {
			task = m_tasks.front();
			m_tasks.pop_front();
			//cout << "Unlocking: " << pthread_self() << endl;
		}
		m_task_mutex.unlock();
		return task;
	}

	int destroy_threadpool() {
		// Note: this is not for synchronization, its for thread communication!
		// destroy_threadpool() will only be called from the main thread, yet
		// the modified m_pool_state may not show up to other threads until its
		// modified in a lock!
		stopped = true;

		cout << "Broadcasting STOP signal to all threads..." << endl;
		m_task_cond_var.broadcast(); // notify all threads we are shutting down

		for (int i = 0; i < m_pool_size; i++) {
			int ret = pthread_join(m_threads[i].self, NULL);
			if(ret!=0) {
				cerr << "pthread_join() returned " << ret << ": " << strerror(errno) << endl;
			}
			m_task_cond_var.broadcast(); // try waking up a bunch of threads that are still waiting
		}
		//cout << m_pool_size << " threads exited from the thread pool" << endl;
		return 0;
	}

	int add_task(ThreadPoolTask* task) {
		m_task_mutex.lock();
		m_tasks.push_back(task);
		m_task_cond_var.signal(); // wake up one thread that is waiting for a task to be available
		m_task_mutex.unlock();
		return 0;
	}

private:
	int m_pool_size;
	Mutex m_task_mutex;
	CondVar m_task_cond_var;
	std::vector<ThreadWork> m_threads; // storage for threads
	std::deque<ThreadPoolTask*> m_tasks;

};

void* ThreadWork::execute_thread() {

	ThreadPoolTask* task = NULL;
	cout << "Starting thread " << self << endl;

	started=true;
	stopped=false;
	executing=false;

	while(started&&!stopped) {

		// Try to pick a task
		cout << "Locking: " << pthread_self() << endl;
		pool->wait();

		// If the thread was woken up to notify process shutdown, return from here
		if (pool->stopped) {
			stopped=true;
		} else if (!stopped) {
			task = pool->popTask();
			if(task!=nullptr) {
				cout << "Executing thread " << pthread_self() << endl;
				// execute the task
				(*task)(); // could also do task->run(arg);
				cout << "Done executing thread " << pthread_self() << endl;
				delete task;
			}
		} else {
			pool->unlock();
		}
	}
	stopped=true;
	return NULL;
}




