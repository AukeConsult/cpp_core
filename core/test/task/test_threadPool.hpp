#pragma once

/*
 * test.h
 *
 *  Created on: Jan 26, 2021
 *      Author: leif
*/

#include <iostream>
#include <pthread.h>
#include "gtest/gtest.h"
#include "../../src/task/ThreadPool.hpp"

using namespace std;

const int MAX_TASKS = 8;

void hello(void* arg) {
  int* x = (int*) arg;
  cout << "Hello " << *x << endl;
  this_thread::sleep_for(5s);
}

TEST(ThreadpoolTest, initPool) {

	  ThreadPool tp(4);
	  int ret = tp.initialize_threadpool();
	  if (ret == -1) {
	    cerr << "Failed to initialize thread pool!" << endl;
	  } else {

		  for (int i = 0; i < MAX_TASKS; i++) {
		    int* x = new int();
		    *x = i+1;
		    ThreadPoolTask* t = new ThreadPoolTask(&hello, (void*) x);
		//    cout << "Adding to pool, task " << i+1 << endl;
		    tp.add_task(t);
		//    cout << "Added to pool, task " << i+1 << endl;
		  }
		  sleep(20);
		  tp.destroy_threadpool();

	  }

	  cout << "Exiting test..." << endl;


}

