// array
#include <array>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <limits>
#include <type_traits>
#include "collection.h"

template <typename Type, typename Array= std::array<Type>>
class ConcurrentArray : Concurrent<Type>, Collection<Type> {

	ConcurrentArray (size_type max_size = std::numeric_limits<size_t>::max()) 
		max_size(max_size), is_shutdown_flag(false), is_terminated_flag(false)	
	{
        if (max_size == 0) {
            throw std::invalid_argument("queue max size is 0");
        }
	}

}