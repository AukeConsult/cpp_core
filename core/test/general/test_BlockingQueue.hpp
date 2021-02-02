#pragma once

#include <exception>
#include <thread>
#include <atomic>
#include "gtest/gtest.h"
#include "../../src/general/Concurrent.hpp"

/*
 * TestTask.h
 *
 *  Created on: Jan 19, 2021
 *      Author: leif
 */

class Dummy {
public:
	Dummy(int f1, int f2, string x) : f1(f1), f2(f2), x(x) {}
	int f1 = 0;
	int f2 = 0;
	string x = "z";
};

TEST(BlockingQueueTest, FirstTest) {

	BlockingQueue<int> q1(5);
	BlockingQueue<int> q2(5);

	q1.push(1);
	q2.push(1);
	q2.push(2);

	ASSERT_EQ((int)q1.size(),1);
	ASSERT_EQ((int)q2.size(),2);

	ASSERT_EQ(q1.take(),1);
	ASSERT_EQ(q2.take(),1);

	ASSERT_EQ((int)q1.size(),0);

	ASSERT_EQ((int)q2.take(),2);
	ASSERT_EQ((int)q2.size(),0);

}

TEST(BlockingQueueTest, PointerTest) {

	BlockingQueue<Dummy*> q1(5);
	BlockingQueue<Dummy*> q2(5);

	q1.push(new Dummy(1,1,"asdasd"));
	q2.push(new Dummy(2,1,"asdasd"));
	q2.push(new Dummy(2,2,"asdasd"));

	ASSERT_EQ((int)q1.size(),1);
	ASSERT_EQ((int)q2.size(),2);

	ASSERT_EQ(q1.take()->f1,1);
	ASSERT_EQ(q2.take()->f1,2);

	ASSERT_EQ((int)q1.size(),0);

	ASSERT_EQ(q2.take()->f2,2);
	ASSERT_EQ((int)q2.size(),0);

}


TEST(BlockingQueueTest, concurrentTest) {


	atomic<int> cnt_sent(0);
	atomic<int> cnt_recived(0);

	BlockingQueue<int> queue(5);


	// create producers
	std::vector<std::thread> producers;
	for (int i = 0; i < 100; i++) {
		producers.push_back(std::thread([&cnt_sent, &queue, i]() {
			cnt_sent++;
			queue.push(i);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}));
	}

	// create consumers
	std::vector<std::thread> consumers;
	for (int i = 0; i < 100; i++) {
		producers.push_back(std::thread([&cnt_recived, &queue, i]() {
			int res = queue.take();
			//cout << "res " << res << endl;
			cnt_recived++;
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}));
	}

	std::for_each(producers.begin(), producers.end(), [](std::thread &thread) {
		thread.join();
	});

	std::for_each(consumers.begin(), consumers.end(), [](std::thread &thread) {
		thread.join();
	});

	ASSERT_EQ(cnt_sent,cnt_recived);

}

TEST(BlockingQueueTest, concurrentObjectTest) {


	atomic<int> cnt_sent(0);
	atomic<int> cnt_recived(0);

	BlockingQueue<Dummy*> queue(5);


	// create producers
	std::vector<std::thread> producers;
	for (int i = 0; i < 10000; i++) {
		producers.push_back(std::thread([&cnt_sent, &queue, i]() {
			cnt_sent++;
			queue.push(new Dummy(i,1,"asdasd"));
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
		}));
	}

	// create consumers
	std::vector<std::thread> consumers;
	for (int i = 0; i < 10000; i++) {
		producers.push_back(std::thread([&cnt_recived, &queue, i]() {
			Dummy * res = queue.take();
			//cout << "res " << res->f1 << endl;
			delete res;
			cnt_recived++;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}));
	}

	std::for_each(producers.begin(), producers.end(), [](std::thread &thread) {
		thread.join();
	});

	std::for_each(consumers.begin(), consumers.end(), [](std::thread &thread) {
		thread.join();
	});

	ASSERT_EQ(cnt_sent,cnt_recived);

}



