//============================================================================
// Name        : unittest.cpp
// Author      : Leif
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "gtest/gtest.h"
using namespace std;

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	cout << "test" << endl;
	return 0;
}
