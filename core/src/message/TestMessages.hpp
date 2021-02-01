#pragma once

/*
 * TestMessage.hpp
 *
 *  Created on: Jan 29, 2021
 *      Author: leif
 */

#include <vector>
#include <string>
#include "BaseMessage.hpp"

using namespace std;
using namespace byteutil;


class Test1: public BaseMessage {

public:

	int 		field1=10;
	int 		field2=132;
	long long 	field3=200321;
	string		field4 = "test message";

	vector<byte> toVector() {
		vector<byte> val = mergeDynamicBytes(
			{
				to_bytes(field1),
				to_bytes(field2),
				to_bytes(field3),
				to_bytes(field4)
			}
		);
		return val;
	}
	void fromVector(vector<byte> * buffer) {
		vector<vector<byte>> val = byteutil::splitDynamicBytes( * buffer);
		field1 = getInt(val[0]);
		field2 = getInt(val[1]);
		field3 = getLong(val[2]);
		field4 = getString(val[3]);
	}

};
