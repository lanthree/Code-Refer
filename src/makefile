include ../Inc.mk

libs = libthread_pool.a libutils.a

all: clean $(libs) install
	

libthread_pool.a: thread_pool.cpp thread_pool.h
	$(C11) -c $<
	$(AR) $(AROPT) $@ thread_pool.o

libutils.a: utils.cpp utils.h
	$(C11) -c $<
	$(AR) $(AROPT) $@ utils.o

install: $(libs)
	mv $(libs) ../libs

clean:
	rm -f *.o
