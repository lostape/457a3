obj-m += CharDevice.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

simpletest:simpletest.cpp
	g++ -o simpletest simpletest.cpp

test:test.cpp
	g++ -o test test.cpp

rw:rw.cpp
	g++ -pthread -o rw rw.cpp
