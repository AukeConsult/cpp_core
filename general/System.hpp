#pragma once
#include <chrono>

using namespace std::chrono;

class System {

public:
	inline static long currentTimeMillis() {
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}
};