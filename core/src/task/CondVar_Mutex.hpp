#pragma once
#include <pthread.h>

class CondVar {
public:
	CondVar(){pthread_cond_init(&m_cond_var, NULL);}
	~CondVar(){pthread_cond_destroy(&m_cond_var);}
	void wait(pthread_mutex_t* mutex) {pthread_cond_wait(&m_cond_var, mutex);}
	void signal(){pthread_cond_signal(&m_cond_var);}
	void broadcast(){pthread_cond_broadcast(&m_cond_var);}
private:
	pthread_cond_t m_cond_var;
};

class Mutex {
public:
	Mutex() {pthread_mutex_init(&m_lock, NULL);is_locked = false;}
	~Mutex() {
		while(is_locked){ unlock(); };  // Unlock Mutex after shared resource is safe
		pthread_mutex_destroy(&m_lock);
	}
	void lock() {pthread_mutex_lock(&m_lock);is_locked = true;}
	void unlock() {is_locked = false; pthread_mutex_unlock(&m_lock);}
	pthread_mutex_t* get_mutex_ptr() {return &m_lock;}
private:
	pthread_mutex_t m_lock;
	volatile bool is_locked;
};
