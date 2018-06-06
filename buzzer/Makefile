KERNELDIR = /lib/modules/$(shell uname -r)/build 

obj-m := buzzer_dev.o

PWD := $(shell pwd)

default : 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
cre_file :
	sudo mknod -m 666 /dev/buzzer_dev c 230 0 
clean :
	rm buzzer_dev.mod.c buzzer_dev.mod.o modules.order Module.symvers *.ko *.o buzzer_app
