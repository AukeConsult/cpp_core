#pragma once

/*
 * BaseMessage.hpp
 *
 *  Created on: Jan 29, 2021
 *      Author: leif
 */

#include "../general/ByteUtil.hpp"

using byte = unsigned char ;

class BaseMessage {

protected:

public:

	virtual ~BaseMessage(){};
	virtual vector<byte> toVector() = 0;
	//virtual void toBytes(byte* buffer, {}short len);
	virtual void fromVector(vector<byte>* buffer) = 0;
	void fromBytes(byte* buffer, int length) {
		vector<byte> v;
		v.resize(length);
		memcpy((void*)&v[0],(void*)buffer,length);
		fromVector(&v);
	}
	byte* toBytes(int &length) {
		vector<byte> v = toVector();
		length=v.size();
		byte* buffer = (byte *) malloc(length);
		memcpy((void*)buffer,&v[0],length);
		return buffer;
	}

};
