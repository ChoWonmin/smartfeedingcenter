KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := btn_dev.o   

PWD := $(shell pwd)

default :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
cre_file :
	sudo mknod -m 666 /dev/btn_dev c 240 0 
clean :  
	find . -type f | xargs touch
	rm -rf $(OBJS)  
	rm -rf btn_dev.mod.c
	rm -rf btn_dev.o   
	rm -rf btn_dev.mod.o
	rm -rf Module.symvers
	rm -rf modules.order

wiringPi:
	gcc -Wall -o btn_wiring  btn_wiringPi.c -lwiringPi

lcd : 
	gcc -o lcd_thread btn_thread.c -lwiringPi -lwiringPiDev  -lpthread

dogFeedingCenter : frs.o buzzer.o main.o
		gcc -o DogFeedingCenter frs.o buzzer.o main.o -lwiringPi -lwiringPiDev -lpthread
frs.o : frs.c
		gcc -c -o frs.o frs.c -lwiringPi
buzzer.o : buzzer.c
		gcc -c -o buzzer.o buzzer.c
main.o : main.c
		gcc -c -o main.o main.c -lwiringPi -lwiringPiDev -lpthread
