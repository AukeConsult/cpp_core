#pragma once

#include "gtest/gtest.h"
#include <exception>
#include "general/concurrent.hpp"

/*
 * TestTask.h
 *
 *  Created on: Jan 19, 2021
 *      Author: leif
 */


class ConcurrentQueueTest : public ::testing::Test {

protected:

	ConcurrentQueue<int> q1;
	ConcurrentQueue<int> q2;

	virtual void SetUp() {

		q1.push(1);
		q2.push(1);
		q2.push(2);

	}

};

TEST_F(ConcurrentQueueTest, FirstTest) {

	ASSERT_EQ((int)q1.size(),1);
	ASSERT_EQ((int)q2.size(),2);

	ASSERT_EQ(q1.pop(),1);
	ASSERT_EQ(q2.pop(),1);

	ASSERT_EQ((int)q1.size(),0);

	ASSERT_EQ((int)q2.pop(),2);
	ASSERT_EQ((int)q2.size(),0);

	try {
		q2.pop();
		FAIL() << "should give exception";
	} catch (SysException & e) {
	}

}

