#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>

#define DEV_PATH "/dev/buzzer_dev"

int main() {

	int fd = 0;

	/* O_NONBLOCK : when opening a FIFO with O_RDONLY or O_WRONLY set */
	if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
		perror("open\n");
		exit(1);
	}

	printf("open success\n");

	close(fd);
	return 0;
}
