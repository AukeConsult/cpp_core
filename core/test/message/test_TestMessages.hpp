#pragma once

/*
 * test_task.hpp
 *
 *  Created on: Jan 26, 2021
 *      Author: leif
 */

#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "../../src/message/TestMessages.hpp"

namespace message {

TEST(MessageTest, simpleMessage) {

	string x="";

	cout << "simple message" << endl;

	Test1* m = new Test1();
	m->field1=99;
	m->field4="0123456789hei på deg 0123456789";
	vector<uint8_t> v = m->toVector();

	cout << "size m " << sizeof(*m) << endl;
	cout << "size v " << v.size() << endl;

	cout << endl;

	Test1 m2;
	m2.fromVector(&v);

	ASSERT_EQ(m->field1,m2.field1);
	ASSERT_EQ(m->field2,m2.field2);
	ASSERT_EQ(m->field3,m2.field3);
	ASSERT_EQ(m->field4,m2.field4);


}

TEST(MessageTest, checkBuffer) {

	cout << "byte buffer test" << endl;

	Test1* m = new Test1();
	m->field1=99;
	m->field4="0123456789hei på deg 0123456789";
	vector<uint8_t> v = m->toVector();
	cout << "size m " << sizeof(*m) << endl;
	cout << "size v " << v.size() << endl;

	int len=0;
	uint8_t* b = m->toBytes(len);
	for(int l=0; l < len; l++) {
		printf("%02x ", *(b+l));
	}
	free(b);
	cout << endl;

	unique_ptr<uint8_t> bt(m->toBytes(len));
	uint8_t* zz = (uint8_t*)bt.get();
	for(int l=0; l < len; l++) {
		printf("%02x ", *(zz+l));
	}
	cout << endl;

}

TEST(MessageTest, bytebuffer) {

	cout << "byte buffer test buffer" << endl;

	Test1* m = new Test1();
	m->field1=99;
	m->field4="0123456789hei på deg 0123456789";
	vector<uint8_t> v = m->toVector();
	cout << "size m " << sizeof(*m) << endl;
	cout << "size v " << v.size() << endl;

	int len;
	unique_ptr<uint8_t> bt(m->toBytes(len));
	uint8_t* zz = (uint8_t*)bt.get();
	for(int l=0; l < len; l++) {
		printf("%02x ", *(zz+l));
	}
	cout << endl;

	Test1 m2;
	m2.fromBytes(zz,len);

	ASSERT_EQ(m->field1,m2.field1);
	ASSERT_EQ(m->field2,m2.field2);
	ASSERT_EQ(m->field3,m2.field3);
	ASSERT_EQ(m->field4,m2.field4);



}


}



