KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m += btn_dev.o   
obj-m += buzzer_dev.o   

PWD := $(shell pwd)

default :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

btn_dev :
	sudo mknod -m 666 /dev/btn_dev c 240 0 

buz_dev :
	sudo mknod -m 666 /dev/buzzer_dev c 230 0 

clean :  
	rm btn_dev.mod.c buzzer.mod.c *.o Module.symvers modules.order center *.ko

wiringPi:
	gcc -Wall -o btn_wiring  btn_wiringPi.c -lwiringPi

dogFeedingCenter : frs.o buzzer.o lcd_module.o main.o
		gcc -o center frs.o buzzer.o lcd_module.o main.o -lwiringPi -lwiringPiDev -lpthread
frs.o : frs.c
		gcc -c -o frs.o frs.c -lwiringPi
buzzer.o : buzzer.c
		gcc -c -o buzzer.o buzzer.c
lcd_module.o : lcd_module.c
		gcc -c -o lcd_module.o lcd_module.c -lwiringPi -lwiringPiDev
main.o : main.c
		gcc -c -o main.o main.c -lwiringPi -lwiringPiDev -lpthread
