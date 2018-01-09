ANLIB  = ranlib
AR     = ar
AROPT  = -crv
C11    = g++ -std=gnu++11

all: clean libthread_pool.a main

main: main.cpp
	$(C11) -o $@ $^ -I./ -L./ -lthread_pool -pthread

libthread_pool.a: thread_pool.cpp thread_pool.h
	$(C11) -c $<
	$(AR) $(AROPT) $@ thread_pool.o

clean:
	rm -f main libthread_pool.a thread_pool.o
