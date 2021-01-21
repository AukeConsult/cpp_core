/*
 * test_typeutil.hpp
 *
 *  Created on: Jan 20, 2021
 *      Author: leif
 */

#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "../../src/general/Byteutil.hpp"

using byte = unsigned char ;

TEST(ByteUtilTest, convertTest) {

	vector<unsigned char> bytes = ByteUtil::to_bytes(100L);
    cout << bytes.size() << endl;

    long val;
	ByteUtil::from_bytes(bytes,val);
    cout << val << endl;

	vector<unsigned char> bytes2 = ByteUtil::to_bytes("123123");
    cout << bytes2.size() << endl;

	for(unsigned char byte : bytes2) {
		printf("%02x ", byte);
	    //cout << byte;
	}

    cout << endl;

    char val2[10];
	ByteUtil::from_bytes(bytes2,val2);
    cout << val2 << endl;

}

TEST(ByteUtilTest, MergeTest) {

	cout << "merge bytes" << endl;

	vector<unsigned char> bytes_m = ByteUtil::mergeBytes({
		ByteUtil::to_bytes(100L),
		ByteUtil::to_bytes(100L),
		ByteUtil::to_bytes(100L),
		ByteUtil::to_bytes(200L)
	});
	cout << bytes_m.size() << endl;

	cout << "split bytes" << endl;
	vector<vector<unsigned char>> bytesarr = ByteUtil::splitBytes(bytes_m,4);
	cout << bytesarr.size() << endl;
	for(vector<unsigned char> bytes : bytesarr) {
		for(unsigned char byte : bytes) {
			printf("%02x ", byte);
		}
	    long val;
		ByteUtil::from_bytes(bytes,val);
	    cout << val << endl;
	}

	cout << "merge dynamic" << endl;

	vector<unsigned char> bytes_dyn = ByteUtil::mergeDynamicBytes({
		ByteUtil::to_bytes(100L),
		ByteUtil::to_bytes(100L),
		ByteUtil::to_bytes("0123456789"),
		ByteUtil::to_bytes(100L),
		ByteUtil::to_bytes(200L),
		ByteUtil::to_bytes("1234567890")
	});
	cout << bytes_dyn.size() << endl;
	for(unsigned char byte : bytes_dyn) {
		printf("%02x ", byte);
	    //cout << byte;
	}
    cout << endl;

	cout << "split dynamic" << endl;

	vector<vector<unsigned char>> bytes_sub = ByteUtil::splitDynamicBytes(bytes_dyn);
	cout << bytesarr.size() << endl;
	for(vector<unsigned char> bytes : bytes_sub) {
	    cout << "var: ";
		for(unsigned char byte : bytes) {
			printf("%02x ", byte);
		}
		cout << endl;
	}

}
