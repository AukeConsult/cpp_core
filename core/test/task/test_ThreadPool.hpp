#pragma once

/*
 * test.h
 *
 *  Created on: Jan 26, 2021
 *      Author: leif
*/

#include <Thread>
#include <atomic>
#include <iostream>

#include "gtest/gtest.h"
#include "../../src/task/ThreadPool.hpp"

using namespace std;

namespace task {

const int MAX_TASKS = 8;

void hello(void* arg) {
  int* x = (int*) arg;
  cout << "Hello " << *x << endl;
  this_thread::sleep_for(2s);
}

TEST(ThreadpoolTest, initPool) {

	  ThreadPool tp(4);

	  for (int i = 0; i < MAX_TASKS; i++) {
		int* x = new int();
		*x = i+1;
		ThreadPoolTask* t = new ThreadPoolTask(&hello, (void*) x);
		tp.add_task(t);
	  }

	  this_thread::sleep_for(10s);

	  for (int i = 9; i < MAX_TASKS+9; i++) {
		int* x = new int();
		*x = i+1;
		ThreadPoolTask* t = new ThreadPoolTask(&hello, (void*) x);
		tp.add_task(t);
	  }

	  this_thread::sleep_for(10s);

	  cout << "Exiting test..." << endl;


}

}

