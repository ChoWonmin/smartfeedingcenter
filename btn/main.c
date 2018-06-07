#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/kdev_t.h>
#include <wiringPi.h>
#include <lcd.h>
#include <pthread.h>
#include <mcp3004.h>
#include <arpa/inet.h>

#include "frs.h"
#include "buzzer.h"
#include "lcd_module.h"

#define BUFFER_LENGTH 256 // buffer for scanning button
#define BTN_DEV_PATH "/dev/btn_dev"
#define CHARMAX 100
#define BUFF_SOCK_MAX 1024 // buffer for data through socket communication

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
char buff_rcv[BUFF_SOCK_MAX + 5]; // data from client

void check_system_time(char* brfst, char* lnch, char* dnr);
void socket_connect(int* server_socket, struct sockaddr_in server_addr);

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
		
		usleep(500000);
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

<<<<<<< HEAD
=======
	
	buzzer_work();
	//buzzer_work();
	//buzzer_work();
	//buzzer_work();

>>>>>>> 7cdb4f8da4ab223e84fc2f7743474facb3018078
	while(1){
	
		weight = getWeight(frc_val_arr,pinBase,FRC_NUM);

		gcvt(weight, 5, buf);
	
		write_lcd(data->lcd, buf);

		// check system time
		check_system_time(data->breakfast, data->lunch, data->dinner);

		sleep(1);
	}
}

// thread function for connectiong with client through socket 
void* sock_func(void* sv_sock)
{
	int client_addr_size;
	int server_socket = *(int*) sv_sock;
	struct sockaddr_in client_addr;
	int client_socket;
	char buff_snd[4] = "get";
	
	while(1)
	{
		puts("accept ready ...");
		client_addr_size = sizeof(client_addr);

		// accept
		client_socket = accept(server_socket, 
				(struct sockaddr*)&client_addr, &client_addr_size);
		if(client_socket == -1)
		{
			printf("fail to accept client  %d  \n", server_socket);
			exit(1);
		}

		puts("accept success");

		// wait until meal time
		while(1)
		{
			if(status == 1)
			{
				// set default flag
				status = 0;
				break;
			}
		}

		sleep(3);

		write(client_socket, buff_snd, strlen(buff_snd) + 1);
		puts("send message to client..");
		read(client_socket, buff_rcv, BUFF_SOCK_MAX);
		printf(" receive : %s\n", buff_rcv);

		// buzzer work three times
		for(int i = 0; i < 3; i++)
		{
			buzzer_work();
		}

		close(client_socket);
	}
}


int main(int argc, char** argv) {

	int lcd;

	pthread_t btn_scan_thread = 0; // button thread
	pthread_t todo_thread = 0; // thread for controlling all sensors.
	pthread_t sock_thread = 0; // thread for socket communication

	tData data; // data arguments for thread function

	// for socket communication
	int server_socket;
	struct sockaddr_in server_addr;

	char buff_rcv[BUFF_SOCK_MAX + 5];

	// check user input 
	if(argc != 4)
	{
		perror("insufficient input\n");
		exit(1);
	}

	// wiring, adc set up & lcd init
	wiringSetup();
	adcSetup(pinBase, spi_num);
<<<<<<< HEAD
	lcd = lcd_init();
=======
	int lcd = lcd_init();
>>>>>>> 7cdb4f8da4ab223e84fc2f7743474facb3018078

	// structure memory allocation
	data.lcd = lcd;
	data.breakfast = (char*)malloc(sizeof(char) * 6);
	strcpy(data.breakfast, argv[1]);
	data.lunch = (char*)malloc(sizeof(char) * 6);
	strcpy(data.lunch, argv[2]);
	data.dinner = (char*)malloc(sizeof(char) * 6);
	strcpy(data.dinner, argv[3]);

	// init socket server and listen
	socket_connect(&server_socket, server_addr);
	
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
			if (todo_thread == 0 || sock_thread == 0) { // todo_thread, sock_thread does not exist 
				// create todo thread
				if ( (pthread_create(&todo_thread, NULL, todo_func, (void *)&data)) < 0) {
					perror("fail to create todo thread");
					exit(1);
				}
				printf("SERVER SOCK %d \n ", server_socket);
				//create sock thread
				if((pthread_create(&sock_thread, NULL, sock_func, (void*)&server_socket)) < 0)
				{
					perror("fail to create sock thread");
					exit(1);
				}
			}
		}
		else { // button off
			printf("off \n");
			if (todo_thread != 0 || sock_thread != 0) { 
				// thread all cancel 
				todo_thread = pthread_cancel(todo_thread);
				sock_thread = pthread_cancel(sock_thread);
			}
			write_lcd(lcd, "");
		}

		usleep(30000);
	}

	return 0;
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

// method that init server sokcet and server listen
void socket_connect(int* server_socket, struct sockaddr_in server_addr)
{
	
	puts("work");
	*server_socket = socket(PF_INET, SOCK_STREAM, 0); // create server socket
	if(*server_socket == -1)
	{
		perror("fail to create server\n");
		exit(1);
	}

	// initiate server setting
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4000);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//binding
	if(bind(*server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind error\n");
		exit(1);
	}

	// listen
	if(listen(*server_socket, 5) == -1)
	{
		perror("fail to listen\n");
		exit(1);
	}

	puts("listening ...");
}
