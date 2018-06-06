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
#include "buzzer.h"

#define BUFFER_LENGTH 256
#define BTN_DEV_PATH "/dev/btn_dev"
#define CHARMAX 100

#define LCD_RS 27
#define LCD_E 28
#define LCD_D4 6
#define LCD_D5 5
#define LCD_D6 4
#define LCD_D7 1

#define pinBase 300
#define spi_num 0

int on = 0; // flag value for checking button on/off
int status = 0; // flag value for checking meal time

int lcd_init();
void write_lcd(int lcd, char* str);
void check_system_time(char* brfst, char* lnch, char* dnr);
typedef struct _todo_arg // structure for arguments of todo thread
{
	int lcd; 
	char* breakfast;
	char* lunch;
	char* dinner;
} tData;

// thread function for scanning button
void *btn_scan(void *data) {
	int fd = 0;
	char receive[BUFFER_LENGTH];

	// button device file open
	if((fd = open(BTN_DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
		perror("btn open()");
        	exit(1);
    	}

	while(1) {

		if (read(fd, receive, BUFFER_LENGTH) < 0) {
			perror("Failed to read... \n");
			exit(1);
		}

		if (strcmp(receive,"click")==0)	on = on?0:1;
		
		sleep(1);
	}

	close(fd);
}

// thread function for controlling all sensors.
void *todo_func(void *args) {

	tData* data = (tData *)args;	

	int frc_val_arr[FRC_NUM];
	int median;	
	float weight;
	char buf[BUFFER_LENGTH];

	
	//buzzer_work();
	//buzzer_work();
	//buzzer_work();
	//buzzer_work();

	while(1){
	
		weight = getWeight(frc_val_arr,pinBase,FRC_NUM);

		gcvt(weight, 5, buf);
	
		write_lcd(data.lcd, buf);

		// check system time
		check_system_time(data.breakfast, data.lunch, data.dinner);

		sleep(1);
	}
}

int main(int argc, char** argv) {

	pthread_t btn_scan_thread = 0; // button thread
	pthread_t todo_thread = 0; // thread for controlling all sensors.

	tData data; // data arguments for thread function

	wiringSetup();
	adcSetup(pinBase, spi_num);
	int lcd = lcd_init();

	// structure memory allocation
	data.lcd = lcd;
	data.breakfast = (char*)malloc(sizeof(char) * 6);
	strcpy(data.breakfast, argv[1]);
	data.lunch = (char*)malloc(sizeof(char) * 6);
	strcpy(data.lunch, argv[2]);
	data.dinner = (char*)malloc(sizeof(char) * 6);
	strcpy(data.dinner, argv[3]);

	// thread which scan button create
	if (pthread_create(&btn_scan_thread, NULL, btn_scan, NULL)< 0) {
	    perror("Button scan thread create error");
	    exit(0);
	}

	// check button on / off	
	while(1){

		// button on 
		if (on) { 
			printf("on \n");
			if (todo_thread == 0) { // todo_thread does not exist 
				// create todo thread
				if ( (pthread_create(&todo_thread, NULL, todo_func, (void *)&data)) < 0) {
					perror("todo thread create error");
					exit(0);
				}
		}
		else { // button off
			printf("off \n");
			if (todo_thread != 0) { 
				// thread all cancel 
				pthread_cancel(todo_thread);
				todo_thread = 0;
			}
			write_lcd(lcd, "");
		}

		sleep(1);
	}

	//pthread_join(btn_scan_thread, NULL);

	return 0;
}

// initiate lcd
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

// display value in lcd
void write_lcd(int lcd, char* str) {
	lcdClear(lcd);
	lcdPosition(lcd,0,0);
	lcdPuts(lcd, str);
}

// method for checking system time
void check_system_time(char* brfst, char* lnch, char* dnr)
{
	time_t t = time(NULL);
	char cur_time[CHARMAX];
	struct tm tm = *localtime(&t);
	sprintf(cur_time, "%d%c%d", tm.tm_hour, ':', tm.tm_min);

	printf("current time: %s\n", cur_time);
		
	if(strcmp(cur_time, brfst) == 0) // when breakfast is commin
	{
		puts("breakfast time");
		status = 1;
		
	}
	else if(strcmp(cur_time, lnch) == 0)// when lunch is comming
	{
		puts("lunch time");
		status = 1;
	}
	else if(strcmp(cur_time, dnr) == 0) // when dinner is comming
	{
		puts("dinner time");
		status = 1;
	}
}
