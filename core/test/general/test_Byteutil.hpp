#pragma once

/*
 * test_typeutil.hpp
 *
 *  Created on: Jan 20, 2021
 *      Author: leif
 */

#include <iostream>
#include <string>
#include <exception>

#include "gtest/gtest.h"
#include "../../src/general/Byteutil.hpp"

TEST(ByteUtilTest, convertTest) {

	vector<uint8_t> bytes_short = byteutil::to_bytes(((short)100));
	for(uint8_t byte : bytes_short) {
		printf("%02x ", byte);
	}
    cout << endl;

    vector<uint8_t> bytes_int = byteutil::to_bytes((int)100);
	for(uint8_t byte : bytes_int) {
		printf("%02x ", byte);
	}
    cout << endl;

    long long xx = 100;
	vector<uint8_t> bytes_long = byteutil::to_bytes(xx);
	for(uint8_t byte : bytes_long) {
		printf("%02x ", byte);
	}
    cout << endl;
	long val;
	byteutil::from_bytes(bytes_long,val);
	ASSERT_EQ(val,100L);

	long val2;
	byteutil::from_bytes(bytes_long,val2);
	ASSERT_EQ(val2,100L);

    cout << val2 << endl;

    try {

    	long  x = byteutil::getLong(bytes_long);
    	cout << x << endl;

    	cout << byteutil::getLong(bytes_long) << endl;
    	cout << byteutil::getLong(bytes_long) << endl;

    	//ASSERT_EQ(ByteUtil::getLong(bytes),100L);

    } catch (exception& ex) {
    	cout << ex.what() << endl;
    }

    cout << "test string" << endl;

	vector<uint8_t> bytes2 = byteutil::to_bytes("123123");
	//ASSERT_EQ((int)bytes2.size(),7);
	for(uint8_t byte : bytes2) {
		printf("%02x ", byte);
	}
    cout << endl;

    char val3[10];
    byteutil::from_bytes(bytes2,val3);
	cout << val3 << endl;

}

TEST(ByteUtilTest, MergeTest) {

	cout << "merge bytes" << endl;

	vector<uint8_t> bytes_m = byteutil::mergeBytes({
		byteutil::to_bytes(100L),
				byteutil::to_bytes(100L),
				byteutil::to_bytes(100L),
				byteutil::to_bytes(100L)
	});
	ASSERT_EQ((int)bytes_m.size(),16);

	cout << "split bytes" << endl;
	vector<vector<uint8_t>> bytes_arr = byteutil::splitBytes(bytes_m,4);
	ASSERT_EQ((int)bytes_arr.size(),4);
	//cout << bytesarr.size() << endl;
	for(vector<uint8_t> bytes : bytes_arr) {
		for(uint8_t byte : bytes) {
			printf("%02x ", byte);
		}
	    long val;
	    byteutil::from_bytes(bytes,val);
		ASSERT_EQ(val,100);
		cout << endl;
	}

	cout << "merge dynamic" << endl;
	vector<uint8_t> bytes_dyn = byteutil::mergeDynamicBytes({
		byteutil::to_bytes(100),
				byteutil::to_bytes(100),
				byteutil::to_bytes("0123456789"),
				byteutil::to_bytes(100),
				byteutil::to_bytes((long long)200),
				byteutil::to_bytes("1234567890")
	});
	for(uint8_t byte : bytes_dyn) {
		printf("%02x ", byte);
	}
    cout << endl;
	cout << bytes_dyn.size() << endl;

	cout << "split dynamic" << endl;

	vector<vector<uint8_t>> bytes_sub = byteutil::splitDynamicBytes(bytes_dyn);
	for(vector<uint8_t> bytes : bytes_sub) {
		for(uint8_t byte : bytes) {
			printf("%02x ", byte);
		}
		cout << endl;
	}

}

TEST(ByteUtilTest, stringTest) {

	cout << "string test" << endl;

	string val="0123456789heipådeg0123456789";
	vector<uint8_t> bytes_short = byteutil::to_bytes(val);
	for(uint8_t byte : bytes_short) {
		printf("%02x ", byte);
	}
    cout << endl;

}
