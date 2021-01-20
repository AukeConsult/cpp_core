#pragma once

#include <iostream>
#include "gtest/gtest.h"
#include "../general/concurrent.hpp"

/*
 * TestTask.h
 *
 *  Created on: Jan 19, 2021
 *      Author: leif
 */

class ConcurrentQueueTest : public ::testing::Test {

public:

	ConcurrentQueue<int> q1;
	ConcurrentQueue<int> q2;

	ConcurrentQueueTest() {
		q1.push(1);
		q2.push(1);
		q2.push(2);
	}

};

TEST_F(ConcurrentQueueTest, FirstTest) {

	cout << "first" << endl;

	ASSERT_EQ((int)q1.size(),1);
	ASSERT_EQ((int)q2.size(),2);

	ASSERT_EQ(q1.pop(),1);
	ASSERT_EQ(q2.pop(),1);

	ASSERT_EQ((int)q1.size(),0);

	ASSERT_EQ((int)q2.pop(),2);
	ASSERT_EQ((int)q2.size(),0);

	q2.pop();

}

