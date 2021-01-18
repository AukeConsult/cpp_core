// concurrent base

#include <mutex>
#include <atomic>
#include <condition_variable>
#include <limits>
#include <type_traits>

class Concurrent {
public:

    void shutdown() {
        lock_guard lock(mx);
        is_shutdown_flag = true;
        cv.notify_all();
    }

    bool is_shutdown() const { return is_shutdown_flag;}
    void terminate() {
        lock_guard lock(mx);
        is_terminated_flag = true;
        cv.notify_all();
    }
    bool is_terminated() const { return is_terminated_flag;}

private:
	mutable std::mutex mx;

protected:
    
	typedef std::lock_guard<std::mutex> lock_guard;

    	std::condition_variable cv;
    	std::atomic_bool is_shutdown_flag;
    	std::atomic_bool is_terminated_flag;


};