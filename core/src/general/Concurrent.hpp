#pragma once
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <exception>
#include <unordered_map>

using namespace std;

class ConcurrentException : public exception {
	string message;
public:
	void* object;
	ConcurrentException() : object(this) {};
	ConcurrentException(const string& what_arg) : message(what_arg), object(this) {}
    virtual string what() throw() {
      return message;
    }
};

template<typename K, typename T>
class ConcurrentHashMap {
private:
    mutex _mutex;
    unordered_map<K, T> _map;
public:
    inline size_t size() {
        unique_lock<mutex> lock(_mutex);
        return _map.size();
    }
    inline void clear() {
        unique_lock<mutex> lock(_mutex);
        unordered_map<K, T> empty;
        swap(_map, empty);
    }
    inline T get(K const& k) {
        unique_lock<mutex> lock(_mutex);
        if(_map.empty()) {
        	throw ConcurrentException("Collection empty");
        }
        return _map[k]; // Return a copy.
    }

//    template<class Value2>
//    void put(Key const& k, Value2&& v) {
//        unique_lock<decltype(mut)> lock(mut);
//        data[k] = forward<Value2>(v);
//    }

    inline void put(K const &k, T &v) {
    	unique_lock<mutex> lock(_mutex);
        _map.insert({k,v});
    }

    inline void remove(K const &k) {
        unique_lock<mutex> lock(_mutex);
        _map.erase(k);
    }

    inline vector<T> values() {
        unique_lock<mutex> lock(_mutex);
        vector<T> v;
        transform(_map.begin(), _map.end(), back_inserter(v),
            [](pair<const K, T> const& p) { return p.second; });
        return v;
    }

    inline vector<K> keys() {
        unique_lock<mutex> lock(_mutex);
        vector<T> v;
        transform(_map.begin(), _map.end(), back_inserter(v),
            [](pair<const K, T> const& p) { return p.first; });
        return v;
    }

};

template<typename T>
class ConcurrentQueue {
private:
	mutex _mutex;
    queue<T> _queue;

public:
    inline ConcurrentQueue<T> () {};
    inline size_t size() {
        unique_lock<mutex> lock(_mutex);
        return _queue.size();
    }
    inline bool empty() {
        unique_lock<mutex> lock(_mutex);
        return _queue.empty();
    }
    inline void clear() {
        unique_lock<mutex> lock(_mutex);
        queue<T> empty;
        swap(_queue, empty);
    }
    inline T peek() {
        unique_lock<mutex> lock(_mutex);
        T v = _queue.front();
        return v;
    }
    inline T pop() {
        unique_lock<mutex> lock(_mutex);
        if(_queue.empty()) {
        	throw ConcurrentException("Collection empty");
        }
        T v = _queue.front();
        _queue.pop();
        return v;
    }
    inline void push(const T& v) {
        unique_lock<mutex> lock(_mutex);
        _queue.push(v);
    }

};


template<typename T>
class BlockingQueue {
private:
    queue<T> _queue;
    size_t _capacity;
    mutex _mutex;
    condition_variable _not_full;
    condition_variable _not_empty;
    bool _release=false;
public:

    inline BlockingQueue() noexcept : _capacity(-1) {};
    inline BlockingQueue(int capacity) noexcept : _capacity(capacity) {};

    inline bool empty() const {
        unique_lock<mutex> lock(_mutex);
        return _queue.empty();
    }

    inline size_t size() const {
        //unique_lock<mutex> lock(_mutex);
        return _queue.size();
    }

    inline void release() {
        unique_lock<mutex> lock(_mutex);
        queue<T> empty;
        swap(_queue, empty);
        _release=true;
        _not_full.notify_all();
    }

    inline void push(const T& elem) {
		unique_lock<mutex> lock(_mutex);
		while (!_release&&(_capacity<0 || _queue.size() >= _capacity)) {
			//cout << "full " << elem << endl;
			_not_full.wait(lock);
		}
		if(!_release) {
			//cout << "pushing " << elem << endl;
			_queue.push(elem);
		}
        _not_empty.notify_all();
    }
    inline const T take() {
		unique_lock<mutex> lock(_mutex);
		while (!_release && _queue.size() == 0) {
			//cout << "empty " << endl;
			_not_empty.wait(lock);
		}
		if(!_release) {
			//cout << "popping element " << _queue.front() << endl;
			T v = _queue.front();
			_queue.pop();
	        _not_full.notify_one();
	        return v;
		} else {
			return *(T*)nullptr;
		}

    }
    inline const T peek() {
        unique_lock<mutex> lock(_mutex);
        while (!_release&&_queue.size() == 0) {
            _not_empty.wait(lock);
        }
        return _queue.front();
    }

};
