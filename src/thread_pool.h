#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <vector>
#include <functional>

namespace lan_tools{

class ThreadPool
{
public:
	typedef std::function<void(void)> Task;

	ThreadPool(int threadNumber);
	~ThreadPool();

	//往任务队列里添加任务
	bool append(Task task);

	//启动线程池
	bool start(void);

	//等待线程池内部的任务全部完成
	bool join();

	//停止线程池
	bool stop(void);

private:
	//线程所执行的工作函数
	void threadLoop(void);

	std::list<Task> tasks_;                                //任务队列
	std::mutex mutex_;                                     //互斥锁

	bool running_;                                         //线程池是否在运行
	int threadNumber_;                                     //线程数
	std::condition_variable_any sleep_until_;              //当任务队列为空时的条件变量 等待被叫醒
	std::vector<std::shared_ptr<std::thread>> threads_;    //用来保存线程对象指针
};

}

#endif
