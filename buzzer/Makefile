KERNELDIR = /lib/modules/$(shell uname -r)/build 

obj-m := buzzer_dev.o

PWD := $(shell pwd)

default : 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
