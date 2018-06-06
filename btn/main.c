#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/kdev_t.h>
#include <wiringPi.h>
#include <lcd.h>
#include <pthread.h>
#include <mcp3004.h>

#include "frs.h"

#define BUFFER_LENGTH 256
#define DEV_PATH "/dev/btn_dev"

#define LCD_RS 27
#define LCD_E 28
#define LCD_D4 6
#define LCD_D5 5
#define LCD_D6 4
#define LCD_D7 1

#define pinBase 300
#define spi_num 0

int on = 0;

int lcd_init();
void write_lcd(int lcd, char* str);

void *btn_scan(void *data) {
	int fd = 0;
	char receive[BUFFER_LENGTH];

	if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
		perror("open()");
        	exit(1);
    	}

	while(1) {

		if (read(fd, receive, BUFFER_LENGTH) < 0) {
			perror("Failed to read... \n");
			exit(1);
		}

		if (strcmp(receive,"click")==0)
			on = on?0:1;
		
		sleep(1);
	}

	close(fd);
}
void *todo_func(void *data) {
	int lcd = *(int *)data;	

	int frc_val_arr[FRC_NUM];
	int median;	
	float weight;
	char buf[BUFFER_LENGTH];

	while(1){
	
		readValue(frc_val_arr, pinBase);
		median = find_median(frc_val_arr, FRC_NUM);
		weight = getWeight(median);

		gcvt(weight, 5, buf);
	
		write_lcd(lcd, buf);

		sleep(1);
	}
}

int main(){
	pthread_t btn_scan_thread = 0;
	pthread_t todo_thread = 0;

	int thr_id;
	int status;

	wiringSetup();
	adcSetup(pinBase, spi_num);
	int lcd = lcd_init();
	
	if (pthread_create(&btn_scan_thread, NULL, btn_scan, NULL)< 0) {
	    perror("Button scan thread create error");
	    exit(0);
	}
	
	while(1){
		if (on) {
			printf("on \n");
			if (todo_thread == 0) { // todo_thread does not exist 
				// create tode thread
				if ( (pthread_create(&todo_thread, NULL, todo_func, (void *)&lcd)) < 0) {
					perror("todo thread create error");
					exit(0);
				}
			} else
				;
		}
		else {
			printf("off \n");
			if (todo_thread != 0) { 
				pthread_cancel(todo_thread);
				todo_thread = 0;
			}
			write_lcd(lcd, "");
		}

		sleep(1);
	}

	pthread_join(btn_scan_thread, (void **)&status);

	return 0;
}
int lcd_init()
{	
	int lcd;

	if(lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0)){
		perror("lcd init failed!\n");
		return -1;
	}
	lcdClear(lcd);

	return lcd;
}
void write_lcd(int lcd, char* str) {
	lcdClear(lcd);
	lcdPosition(lcd,0,0);
	lcdPuts(lcd, str);
}
