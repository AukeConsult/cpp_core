#include <iostream>
#include <string>
#include <mutex>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include <chrono>
#include <random>
#include <thread>
#include "general/Random.hpp"
#include "general/Concurrent.hpp"

//#include "task/ServiceMonitor.hpp"
#include "task/Task.hpp"
//#include "task/TaskMonitor.hpp"

using namespace std;

using Random = effolkronium::random_static;

class testTask : public Task {
	// Inherited via Task
public:
	testTask(int f) : Task(f) {};
	~testTask(){};
	virtual void onStart() override {}
	virtual void onExecute() override {
		cout << iD << endl;
	}
	virtual void onStop() override {}
};

int main(int argc, char **argv) {

	ConcurrentQueue<int> test;

	cout << "Hello world" << endl;

	//test.push(21);
	//test.push(22);

	//ExecutorService threadpool;
	//TaskMonitor* pingMonitor = new TaskMonitor(&threadpool, 100, "pingMonitor ");
	//pingMonitor->execute(new testTask(10));

	//cout << test.pop() << endl;
	//cout << test.pop() << endl;

	ConcurrentQueue<testTask*> executelist;

	executelist.push(new testTask(1));
	executelist.push(new testTask(2));
	executelist.push(new testTask(3));
	executelist.push(new testTask(4));

	while (!executelist.empty())
	{
		std::cout << executelist.pop()->frequency << '\n';
	}


	cout << "Hello world end" << endl;


}


