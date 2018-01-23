include ../Inc.mk

libs = libthread_pool.a libutils.a liblog.a

all: clean $(libs) install
	

libthread_pool.a: thread_pool.cpp thread_pool.h
	$(C11) -c $<
	$(AR) $(AROPT) $@ thread_pool.o

libutils.a: utils.cpp utils.h
	$(C11) -c $<
	$(AR) $(AROPT) $@ utils.o

liblog.a: log.cpp log.h
	$(C11) -c $<
	$(AR) $(AROPT) $@ log.o

install: $(libs)
	mv $(libs) ../libs

clean:
	rm -f *.o
