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

namespace message {

using namespace std;
using namespace byteutil;

class Test1: public BaseMessage {

public:

	int 			field1=10;
	int 			field2=132;
	long long 		field3=200321;
	string			field4 = "test message";
	vector<uint8_t> 	field5;

	vector<uint8_t> toVector() {
		vector<uint8_t> val = mergeDynamicBytes(
			{
				to_bytes(field1),
				to_bytes(field2),
				to_bytes(field3),
				to_bytes(field4),
				field5
			}
		);
		return val;
	}
	void fromVector(vector<uint8_t> * buffer) {
		vector<vector<uint8_t>> val = byteutil::splitDynamicBytes( * buffer);
		field1 = getInt(val[0]);
		field2 = getInt(val[1]);
		field3 = getLong(val[2]);
		field4 = getString(val[3]);
		field5 = val[4];
	}

};

}
