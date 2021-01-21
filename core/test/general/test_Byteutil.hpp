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

	vector<unsigned char> bytes2 = ByteUtil::to_bytes("1234567890");
    cout << bytes2.size() << endl;

    char val2[10];
	ByteUtil::from_bytes(bytes2,val2);
    cout << val2 << endl;

}

TEST(ByteUtilTest, MergeTest) {

	vector<unsigned char> bytesm = ByteUtil::mergeBytes({
			ByteUtil::to_bytes(100L),
			ByteUtil::to_bytes(100L),
			ByteUtil::to_bytes(100L),
			ByteUtil::to_bytes(200L)
	});
	cout << bytesm.size() << endl;


	vector<vector<unsigned char>> bytesarr = ByteUtil::splitBytes(bytesm,4);
	cout << bytesarr.size() << endl;

	for(vector<unsigned char> bytes : bytesarr) {
	    long val;
		ByteUtil::from_bytes(bytes,val);
	    cout << val << endl;
	}


}
