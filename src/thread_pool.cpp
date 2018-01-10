#include "thread_pool.h"

ThreadPool::ThreadPool(int threadNumber)
	:threadNumber_(threadNumber),
	 running_(true),
	 threads_(0) { }

ThreadPool::~ThreadPool()
{
	if (running_) {
		stop();
	}
}


// make_shared
//	http://blog.csdn.net/xiejingfa/article/details/50750037
//	会在堆中分配一个对象并初始化 最后返回指向此对象的shared_ptr实例
// 	传递给make_shared函数的参数必须和shared_ptr所指向类型的某个构造函数相匹配
//
// bind
//	https://segmentfault.com/a/1190000003897709
//
// thread
//	http://www.mamicode.com/info-detail-1396559.html
bool ThreadPool::start(void)
{
	for (int i = 0; i < threadNumber_; ++i) {
		/*threads_.push_back(
			std::make_shared<std::thread>(
				std::bind(&ThreadPool::threadLoop, this)
			)
		);*/
		//循环创建线程
		threads_.push_back(
			std::make_shared<std::thread>(&ThreadPool::threadLoop, this)
		);
	}
	return true;
}

bool ThreadPool::join(void)
{
	while (!tasks_.empty()) {}
	stop();
	return true;
}

bool ThreadPool::stop(void)
{
	if (running_) {
		running_ = false;
		// 叫醒阻塞的线程 让它去死...
		while (threadNumber_) {
			sleep_until_.notify_all();
		}

		//确保线程安全退出
		for (auto &t : threads_) {
			t->join();
		}
	}
	
	return true;
}

bool ThreadPool::append(Task task)
{
	std::lock_guard<std::mutex> guard(mutex_);
	tasks_.push_back(task);         //将该任务加入任务队列
	sleep_until_.notify_one();      //唤醒某个线程来执行此任务
	return true;
}

void ThreadPool::threadLoop(void)
{
	Task task = nullptr;
	while(running_) {
		{
			std::lock_guard<std::mutex> guard(mutex_);
			if (tasks_.empty()) {
				sleep_until_.wait(mutex_); //等待有任务到来被唤醒
			}

			if (!tasks_.empty()) {
				task = tasks_.front();     //从任务队列中获取最开始任务
				tasks_.pop_front();	   //将取走的任务弹出任务队列
			} else {
				continue;
			}
		}
		task();   //执行任务
	}
	-- threadNumber_; //计数 确保正确结束
}
