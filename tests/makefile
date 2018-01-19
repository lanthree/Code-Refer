include ../Inc.mk

test = libthread_pool.test libutils.test

all: clean $(test)
	

libthread_pool.test: thread_pool_test.cpp
	$(C11) -o $@ $^ -I../src -L../libs -lthread_pool -pthread

libutils.test: utils_test.cpp
	$(C11) -o $@ $^ -I../src -L../libs -lutils -pthread
clean:
	rm -f *.o $(test)
