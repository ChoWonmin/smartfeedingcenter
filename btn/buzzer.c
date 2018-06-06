#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fcntl.h>
#include <linux/kdev_t.h>
#include <unistd.h>
#include <sys/types.h>

#include "buzzer.h"

// method that buzzer work
void buzzer_work()
{
	int fd, ret;
	char message[5] = "buzz";

	// open buzzer device file
	if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0)
	{
		perror("fail to open buzzer device file\n");
		exit(1);
	}

	// write message to device file for working buzzer
	ret = write(fd, message, (strlen(message) + 1));
	if(ret < 0)
	{
		perror("fail to write \"buzz\" message to the device\n");
		exit(1);
	}

	puts("open buzz device file success!!\n");
	close(fd);
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

		printf("current time : %s\n", cur_time);

		if(strcmp(cur_time, meal_time[1]) == 0) // breakfast comming
		{
			puts("breakfast time!!");
			status = 1;
		}
		else if(strcmp(cur_time, meal_time[2]) == 0) // breakfast comming
		{
			puts("lunch time!!");
			status = 1;
		}
		if(strcmp(cur_time, meal_time[3]) == 0) // breakfast comming
		{
			puts("dinner time!!");
			status = 1;
		}
		sleep(60);
	}
}


