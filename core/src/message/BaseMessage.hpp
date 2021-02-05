#pragma once

/*
 * BaseMessage.hpp
 *
 *  Created on: Jan 29, 2021
 *      Author: leif
 */

#include "../general/ByteUtil.hpp"

namespace message {

class BaseMessage {

protected:

public:

	virtual ~BaseMessage(){};
	virtual vector<uint8_t> toVector() = 0;
	//virtual void toBytes(uint8_t* buffer, {}short len);
	virtual void fromVector(vector<uint8_t>* buffer) = 0;
	void fromBytes(uint8_t* buffer, int length) {
		vector<uint8_t> v;
		v.resize(length);
		memcpy((void*)&v[0],(void*)buffer,length);
		fromVector(&v);
	}
	uint8_t* toBytes(int &length) {
		vector<uint8_t> v = toVector();
		length=v.size();
		uint8_t* buffer = (uint8_t *) malloc(length);
		memcpy((void*)buffer,&v[0],length);
		return buffer;
	}

};


}
