
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
	byte* b = (byte*) test.data();
	for(unsigned int l=0; l < test.length(); l++) {
		printf("%02x ", *(b+l));
	}
	cout << endl;

	int outlen;
	byte* ret_encr = cipher.enCrypt((byte*)test.data(), test.length(), outlen);
	for(int l=0; l < outlen; l++) {
		printf("%02x ", *(ret_encr+l));
	}
	cout << endl << "end e " << outlen << endl;

	int retlen;
	byte* ret_decr = cipher.deCrypt(ret_encr, outlen, retlen);
	for(int l=0; l < retlen; l++) {
		printf("%02x ", *(ret_decr+l));
	}
	cout << endl << "end d " << retlen << endl;

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

		int outlen;
		byte* ret_encr = cipher.enCrypt((byte*)in.data(), in.length(), outlen);

		int retlen;
		byte* ret_decr = cipher.deCrypt(ret_encr, outlen, retlen);

		std::string out((char*)ret_decr);
		cout << out << endl;
		cout << "end d " << retlen << endl;

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

		int outlen;
		byte* ret_encr = factory.enCrypt((byte*)in.data(), in.length(), outlen);

		int retlen;
		byte* ret_decr = factory.deCrypt(ret_encr, outlen, retlen);

		std::string out((char*)ret_decr);
		//cout << "size" << " " << outlen << " " << retlen << endl;

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

				int outlen;
				byte* ret_encr = factory.enCrypt((byte*)in.data(), in.length(), outlen);

				int retlen;
				byte* ret_decr = factory.deCrypt(ret_encr, outlen, retlen);

				std::string out((char*)ret_decr);

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
