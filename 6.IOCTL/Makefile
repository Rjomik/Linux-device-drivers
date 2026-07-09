obj-m += ioctl.o

all: module testApp
	
module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

testApp: test.c
	gcc test.c -o test

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm test