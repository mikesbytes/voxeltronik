/*

Thread pool

 */

#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace vtk {

class ThreadPool {
private:
	ThreadPool();
	ThreadPool(ThreadPool const& copy);
	ThreadPool& operator=(ThreadPool const& copy);

public:
	static ThreadPool& getInstance();

	void addThreads(const int& count); // add new threads to the pool
	void addJob(std::function<void()> newJob); // add a job to the pool

	int threadCount();

private:
	void threadIdle();

	std::vector<std::thread> mThreads;
	std::queue<std::function<void()> > mQueue;
	std::mutex mQueueMutex;
	std::condition_variable mCondition;
};

}
