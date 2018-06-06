#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/kdev_t.h>
#include <time.h>
#include <pthread.h>

#define CHARMAX 100
#define BUFF_SIZE 1024
#define DEV_PATH "/dev/buzzer_dev"

void buzzer_work();
void socket_connect(int* server_socket, struct sockaddr_in server_addr);
void *check_system_time(void* argv);

int status = 0; // flag value that check meal time

int main( int argc, char** argv) {

	char cur_time[CHARMAX]; // current system time

	pthread_t thread; //thread for checking System time

	// for socket communication
	int client_socket;
	int server_socket;
	int client_addr_size;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	// data receive from client or sent to client 
	char buff_rcv[BUFF_SIZE + 5]; 
	char send_msg[4] = "get"; 

	if(argc != 4)
	{
		printf("insufficient input\n");	
		exit(1);
	}

	// server connect to client
	socket_connect(&server_socket, server_addr);

	// thread create
	if(pthread_create(&thread, NULL, check_system_time, (void*)argv) < 0)
	{
		perror("thread creaet error\n");
		exit(1);
	}
	puts("checking System time..");

	// process start
	while(1)
	{
		puts("accept ready");
		client_addr_size = sizeof(client_addr);
		// accept
		client_socket = accept(server_socket, 
					(struct sockaddr*)&client_addr, &client_addr_size);
		if(client_socket == -1)
		{
			puts("failed to accept client");
			exit(1);
		}

		puts("accept success"); // socket connect 

		while(1)
		{ 
			if(status == 1)
			{
				// initaite flag
				status = 0;
				break;
			}
		} // wait until meal time
		

		sleep(5);

		write(client_socket, send_msg, strlen(send_msg) + 1);
		puts("write success");

		read(client_socket, buff_rcv, BUFF_SIZE);
		printf("receive : %s\n", buff_rcv);

		close(client_socket);
	}

	return 0;
}

// method that buzzer work
void buzzer_work()
{
	int fd, ret;
	char message[5] = "buzz";
	/* O_NONBLOCK : when opening a FIFO with O_RDONLY or O_WRONLY set */
	if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
		printf("failed to open device file\n");
		exit(1);
	}

	//write "buzz" to device file for working buzzer
	ret = write(fd, message, (strlen(message)+1));
	if(ret < 0)
	{
		printf("Failed to write the message to the device\n");
		exit(1);
	}

	printf("open success\n");
	close(fd);
}

// connect with clien rasp through socket and communicate
void socket_connect(int* server_socket, struct sockaddr_in server_addr)
{
	*server_socket = socket(PF_INET, SOCK_STREAM, 0); // creat server socket
	if(*server_socket == -1) 
	{
		printf("failed to create server\n");
		exit(1);
	}

	// initiate server setting
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4000);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// binding
	if(bind(*server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		printf("bind execute error\n");
		exit(1);
	}

	// listen
	if(listen(*server_socket, 5) == -1) 
	{
		printf("failed to listen\n");
		exit(1);
	}

	puts("listening..");
}

// thread function that check system time
void *check_system_time(void* argv)
{
	char** meal_time = (char **) argv;
	// get system time 
	// check that system time is coincides with each user input every minute
	while(1)
	{
		time_t t = time(NULL);
		char cur_time[CHARMAX];
		int result = 0;
		struct tm tm = *localtime(&t);
		sprintf(cur_time, "%d%c%d", tm.tm_hour, ':', tm.tm_min);

		printf("current time: %s\n", cur_time);
		
		if(strcmp(cur_time, meal_time[1]) == 0) // when breakfast is comming
		{
			puts("breakfast buzzer!");
			//result = 1;
			//return (void*)result;
			status = 1;
		}
		else if(strcmp(cur_time, meal_time[2]) == 0)// when lunch is comming
		{
			puts("lunch buzzer");
			//result = 1;
			//return (void*)result;
			status = 1;
		}
		else if(strcmp(cur_time, meal_time[3]) == 0) // when dinner is comming
		{
			puts("dinner buzzer");
			//result = 1;
			//return (void*)result;
			status = 1;
		}
	
		sleep(60);
	}

}




