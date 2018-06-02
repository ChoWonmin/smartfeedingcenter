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

void buzzerWork();

int main( int argc, char** argv) {

	char cur_time[CHARMAX]; // current system time
	char* breakfast;
	char* lunch;
	char* dinner;
	
	// receivce breakfast, lunch, dinner from user
	if( argc != 4)
	{
		printf("insufficient input\n");	
		exit(1);
	}
	
	breakfast = malloc(sizeof(char) * strlen(argv[1]));
	lunch = malloc(sizeof(char) * strlen(argv[2]));
	dinner = malloc(sizeof(char) * strlen(argv[3]));

	strcpy(breakfast, argv[1]);
	strcpy(lunch, argv[2]);
	strcpy(dinner, argv[3]);

	// get system time 
	// check that system time is coincides with each user input every minute
	while(1)
	{
		time_t t = time(NULL);
		char cur_time[CHARMAX];
		struct tm tm = *localtime(&t);
		sprintf(cur_time, "%d%c%d", tm.tm_hour, ':', tm.tm_min);

		printf("current time: %s\n", cur_time);
		
		if(strcmp(cur_time, breakfast) == 0) // when breakfast is comming
		{
			puts("breakfase buzzer!\n");
			buzzerWork();
			break;
		}
		else if(strcmp(cur_time, lunch) == 0)// when lunch is comming
		{
			puts("lunch buzzer\n");
			buzzerWork();
			break;
		}
		else if(strcmp(cur_time, dinner) == 0) // when dinner is comming
		{
			puts("dinner buzzer\n");
			buzzerWork();
			break;
		}
	
		sleep(60);
	}

	close(fd);
	free(breakfast);
	free(lunch);
	free(dinner);
	return 0;
}

// method that buzzer work
void buzzerWork()
{
	/* O_NONBLOCK : when opening a FIFO with O_RDONLY or O_WRONLY set */
	if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
		perror("open\n");
		exit(1);
	}

	printf("open success\n");
}
