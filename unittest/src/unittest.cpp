//============================================================================
// Name        : unittest.cpp
// Author      : Leif
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "gtest/gtest.h"

#include "test_general/test.h"

using namespace std;

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	cout << "test" << endl;



	return RUN_ALL_TESTS();
}
