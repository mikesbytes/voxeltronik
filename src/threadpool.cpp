#include "threadpool.h"

namespace vtk {

ThreadPool::ThreadPool() {
}

ThreadPool::ThreadPool(ThreadPool const& copy) {

}

ThreadPool& ThreadPool::operator=(ThreadPool const& copy) {
	return *this;
}

ThreadPool& ThreadPool::getInstance() {
	static ThreadPool instance;
	return instance;
}

void ThreadPool::addThreads(const int &count) {
	for (int i = 0; i < count; ++i) {
		mThreads.push_back(std::thread(&ThreadPool::threadIdle, this));
	}
}

void ThreadPool::threadIdle() {

	// Idle until something appears in the job queue, then run it
	
	std::function<void()> job;
	while(true) {
		{
			std::unique_lock<std::mutex> lock(mQueueMutex);
			mCondition.wait(lock, [&]{return !mQueue.empty();});

			job = mQueue.front();
			mQueue.pop();
		}
		job();
	}
}

void ThreadPool::addJob(std::function<void ()> newJob) {
	{
		std::unique_lock<std::mutex> lock(mQueueMutex);
		mQueue.push(newJob);
	}
	mCondition.notify_one();
}
			
}
