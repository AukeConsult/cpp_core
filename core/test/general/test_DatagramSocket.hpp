#pragma once

/*
 * test_typeutil.hpp
 *
 *  Created on: Jan 20, 2021
 *      Author: leif
 */

#include <iostream>
#include <string>
#include <thread>
#include "gtest/gtest.h"
#include "../../src/general/DatagramSocket.h"

using namespace std;

namespace udpcom {

TEST(DatagramTest, initDatagram) {

	DatagramSocket* r = new DatagramSocket(9999,false,false);
	thread t([&r] {
		char msg[4000];
		char address[30];
		int port;
		int retlen=0;
		while(retlen>-1) {
			retlen = r->receive(msg, 4000, address, port);
			cout << msg << " len: " << retlen << " addr: " << address << ":" << port << endl;
		}
		cout << "end socket send " << WSAGetLastError() << endl;

	});

	DatagramSocket* s = new DatagramSocket(9998,false,false);

	const char * address = s->getAddress("localhost");

	for(int i=0;i<10;i++) {
		string x = "01234567890123456789-" + to_string(i);
		long ret = s->sendTo(x.c_str(),x.length(),address,9999);
	    if(ret==-1){
	        printf("The error is %d", WSAGetLastError());
	    }
	}
	r->close();
	t.join();
}

}
