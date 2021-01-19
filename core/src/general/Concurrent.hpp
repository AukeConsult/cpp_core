#pragma once
#include <mutex>
#include <algorithm>
#include <mutex>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

template<class Key, class Value>
class ConcurrentHashMap
{
    mutex mut;
    unordered_map<Key, Value> data;

public:
    
    size_t size() {
        unique_lock<decltype(mut)> lock(mut);
        return data.size();
    }

    void clear() {
        unique_lock<decltype(mut)> lock(mut);
        unordered_map<Key, Value> empty;
        swap(data, empty);
    }

    
    Value get(Key const& k) {
        unique_lock<decltype(mut)> lock(mut);
        return data[k]; // Return a copy.
    }

    template<class Value2>
    void put(Key const& k, Value2&& v) {
        unique_lock<decltype(mut)> lock(mut);
        data[k] = forward<Value2>(v);
    }

    void remove(Key const& k) {
        unique_lock<decltype(mut)> lock(mut);
        data.erase(k);
    }

    vector<Value> values() {
        unique_lock<decltype(mut)> lock(mut);
        vector<Value> v;
        transform(data.begin(), data.end(), back_inserter(v),
            [](pair<const Key, Value> const& p) { return p.second; });
        return v;
    }

    vector<Key> keys() {
        unique_lock<decltype(mut)> lock(mut);
        vector<Value> v;
        transform(data.begin(), data.end(), back_inserter(v),
            [](pair<const Key, Value> const& p) { return p.first; });
        return v;
    }

};

template<class Value>
class ConcurrentQueue
{
    mutex mut;
    queue<Value> data;

public:

    size_t size() {
        unique_lock<decltype(mut)> lock(mut);
        return data.size();
    }
    
    bool empty() {
        unique_lock<decltype(mut)> lock(mut);
        return data.empty();
    }

    void clear() {
        unique_lock<decltype(mut)> lock(mut);
        queue<Value> empty;
        swap(data, empty);
    }

    Value peek() {
        unique_lock<decltype(mut)> lock(mut);
        Value v = data.front();
        return v;
    }

    Value pop() {
        unique_lock<decltype(mut)> lock(mut);
        Value v = data.front();
        data.pop();
        return v;
    }

    void push(const Value& v) {
        unique_lock<decltype(mut)> lock(mut);
        data.push(v);
    }

    //void push(const Value&& v) {
    //    unique_lock<decltype(mut)> lock(mut);
    //    data.push(forward<Value>(v));
    //}

    void emplace(const Value& v) {
        unique_lock<decltype(mut)> lock(mut);
        data.emplace(v);
    }

    void emplace(const Value&& v) {
        unique_lock<decltype(mut)> lock(mut);
        data.emplace(forward<Value>(v));
    }

};
