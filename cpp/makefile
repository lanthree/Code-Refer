include ./Inc.mk

all:
	[ -d "$(WORKHOME)/libs" ] || mkdir  "$(WORKHOME)/libs"
	make -C src

test:
	make -C tests

clean:
	rm -f ./libs/*.a
	make clean -C src
	make clean -C tests
