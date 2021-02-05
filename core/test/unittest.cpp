//============================================================================
// Name        : unittest.cpp
// Author      : Leif
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "gtest/gtest.h"

#include "encryption/test.h"
#include "general/test.h"
#include "task/test.h"
#include "message/test.h"

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	std::cout << "test" << std::endl;
	return RUN_ALL_TESTS();
}
