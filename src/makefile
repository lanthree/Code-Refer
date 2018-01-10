include ../Inc.mk

libs = libthread_pool.a

all: clean $(libs) install
	

libthread_pool.a: thread_pool.cpp thread_pool.h
	$(C11) -c $<
	$(AR) $(AROPT) $@ thread_pool.o

install: $(libs)
	mv $(libs) ../libs

clean:
	rm -f *.o
