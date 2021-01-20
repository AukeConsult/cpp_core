/*
 * test_typeutil.hpp
 *
 *  Created on: Jan 20, 2021
 *      Author: leif
 */

#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include "general/byteutil.hpp"


using byte = unsigned char ;

TEST(ByteUtilTest, convertTest) {

	long val = 100;
	vector<unsigned char > bytes = ByteUtil::to_bytes(val);
    int l = bytes.size();
    cout << l << endl;

    cout << strlen("aasdadsasdadsasdasd") << endl;

	vector<unsigned char> bytes2 = ByteUtil::to_bytes("asswdsdadsdasddasdasdasd");
    cout << "test2 " << bytes2.size() << endl;


    //long v = ByteUtil::from_bytes(bytes, (long)1);

    //cout << ByteUtil::fromBytes(ByteUtil::toBytes(val)) << endl;
    //cout << ByteUtil::fromBytes(ByteUtil::toBytes("asdasd")) << endl;


}

TEST(ByteUtilTest, MergeTest) {


}
