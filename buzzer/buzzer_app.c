#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>
#include <time.h>

#define CHARMAX 100
#define DEV_PATH "/dev/buzzer_dev"
#define BREAKFAST "09:00"
#define LUNCH "12:00"
#define DINNER "19:00"

int main( int argc, char** argv) {

	int fd = 0;
	char cur_time[CHARMAX]; // current system time
	
	// receivce breakfast, lunch, dinner from user
	if( argc != 4)
	{
		printf("insufficient input\n");	
		exit(1);
	}

	// get system time 
	// check that system time is coincides with each user input every minute
	while(1)
	{
		time_t t = time(NULL);
		char cur_time[CHARMAX];
		struct tm tm = *localtime(&t);
		sprintf(cur_time, "%d%c%d", tm.tm_hour, ':', tm.tm_min);

		printf("current time: %s\n", cur_time);
		
		if(strcmp(cur_time, BREAKFAST) == 0) // when breakfast is comming
		{
			puts("breakfase buzzer!\n");
			break;
		}
		else if(strcmp(cur_time, LUNCH) == 0)// when lunch is comming
		{
			puts("lunch buzzer\n");
			break;
		}
		else // when dinner is comming
		{
			puts("dinner buzzer\n");
			break;
		}
	
		// lunch
		// dinner
		sleep(60);
	}

	/* O_NONBLOCK : when opening a FIFO with O_RDONLY or O_WRONLY set */
	//if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
		//perror("open\n");
		//exit(1);
	//}

	//printf("open success\n");

	//close(fd);
	return 0;
}
