
/*
 * test_1.hpp
 *
 *  Created on: Jan 26, 2021
 *      Author: leif
 */

#pragma once

#include <thread>
#include <random>
#include <string>
#include <iterator>
#include <algorithm>
#include <random>
#include "gtest/gtest.h"

#include "../../src/general/Random.hpp"
#include "../../src/encryption/M2Encryption.hpp"

using namespace std;
using namespace encryption;

TEST(EncryptionTest, CipherBase) {

	CipherBase cipher;

	string test = "0123456789012345678901234567890";

	cout << "st  " << test.length() << endl;
	uint8_t* b = (uint8_t*) test.data();
	for(unsigned int l=0; l < test.length(); l++) {
		printf("%02x ", *(b+l));
	}
	cout << endl;

	vector<uint8_t>* data = new vector<uint8_t>(test.begin(),test.end());
	cout << data->size() << endl;

	vector<uint8_t>* ret_encr = cipher.enCrypt(data);
	for(int l=0; l < (int)ret_encr->size(); l++) {
		printf("%02x ", (*ret_encr)[l]);
	}
	cout << endl << "end e " << ret_encr->size() << endl;

	vector<uint8_t>* ret_decr = cipher.deCrypt(ret_encr);
	for(int l=0; l < (int)ret_decr->size(); l++) {
		printf("%02x ", (*ret_decr)[l]);
	}
	cout << endl << "end d " << ret_decr->size() << endl;

	delete ret_encr;
	delete ret_decr;

}


TEST(EncryptionTest, CipherBaseMany) {

	CipherBase cipher;
	using Random = effolkronium::random_static;

	string in = "012345678901234567890123456789012345678901234567890";

	cout << "start " << endl;

	for(int i=1;i<10;i++) {

		generate_n(in.begin(), in.length(), [] () -> char {return Random::get('a','z');});
		cout << in << endl;

		vector<uint8_t>* data = new vector<uint8_t>(in.begin(),in.end());
		vector<uint8_t>* ret_encr = cipher.enCrypt(data);
		vector<uint8_t>* ret_decr = cipher.deCrypt(ret_encr);
		std::string out(ret_decr->begin(), ret_decr->end());

		cout << out << endl;

		ASSERT_EQ(in,out);

		delete ret_encr;
		delete ret_decr;

	}
	cout << endl;
}

TEST(EncryptionTest, EncryptionFactory) {

	EncryptFactory factory;
	using Random = effolkronium::random_static;

	cout << "start " << endl;

	for(int i=1;i<100;i++) {

		string in;
		in.resize(Random::get(10000,100000), '0');

		generate_n(in.begin(), in.length(), [] () -> char {return Random::get('0','Z');});
		vector<uint8_t>* data = new vector<uint8_t>(in.begin(),in.end());
		vector<uint8_t>* ret_encr = factory.enCrypt(data);
		vector<uint8_t>* ret_decr = factory.deCrypt(ret_encr);
		std::string out(ret_decr->begin(), ret_decr->end());
		ASSERT_EQ(in,out);

		delete ret_encr;
		delete ret_decr;

	}
	cout << endl;

}

TEST(EncryptionTest, EncryptionFactoryThread) {

	EncryptFactory factory;
	using Random = effolkronium::random_static;

	cout << "start " << endl;

	vector<thread> producers;
	for (int x = 0; x < 100; x++) {
		producers.push_back(thread([&]() {

			for(int i=1;i<100;i++) {

				string in;
				in.resize(Random::get(10000,100000), '0');

				generate_n(in.begin(), in.length(), [] () -> char {return Random::get('0','Z');});
				vector<uint8_t>* data = new vector<uint8_t>(in.begin(),in.end());
				vector<uint8_t>* ret_encr = factory.enCrypt(data);
				vector<uint8_t>* ret_decr = factory.deCrypt(ret_encr);
				std::string out(ret_decr->begin(), ret_decr->end());

				ASSERT_EQ(in,out);

				delete ret_encr;
				delete ret_decr;

				std::this_thread::sleep_for(std::chrono::milliseconds(1));

			}


		}));
	}

	for_each(producers.begin(), producers.end(), [](thread &thread) {
		thread.join();
	});

	cout << endl;

}
