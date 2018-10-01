#include "thread_pool.h"
#include <iostream>
#include <mutex>
#include <unistd.h>
using namespace std;

std::mutex g_mutex;
int counter1 = 0;

void func1()
{
	std::lock_guard<std::mutex> guard(g_mutex);
	counter1 += 1;
}


int counter2 = 0;
void func2(int num)
{
	std::lock_guard<std::mutex> guard(g_mutex);
	counter2 += num;
}

int main()
{
	lan_tools::ThreadPool pool1(10);
	pool1.start();
	int cnt = 0;
	while (++cnt <= 100000) {
		pool1.append(func1);
	}
	pool1.join();
	cout << counter1 << endl;


	lan_tools::ThreadPool pool2(10);
	pool2.start();
	cnt = 0;
	while (++cnt <= 1000) {
		pool2.append( bind(func2, cnt) );
	}
	pool2.join();
	cout << counter2 << endl;

	return 0;
}
