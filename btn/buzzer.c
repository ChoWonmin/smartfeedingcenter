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
	if((fd = open(BUZ_DEV_PATH, O_RDWR | O_NONBLOCK)) < 0)
	{
		perror("fail to open buzzer device file\n");
		exit(1);
	}

	// write message to device file for working buzzer
	ret = write(fd, message, (strlen(message) + 1));
	if(ret < 0)
	{
		perror("fail to write \"buzz\" message to the device \n");
		exit(1);
	}

	puts("open buzz device file success!!\n");
	close(fd);
}


