#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>

#define DEV_PATH "/dev/btn_dev"

int main(int argc, char *argv[]) {
  int fd, input = 0;

  while(1) {
    if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
      perror("open()");
      exit(1);
    }

    printf("clicked!!!!!\n");
    
    printf("continue??? 0 or anything :: ");
    scanf(" %d", &input);
    
    if(input==0)
	break;    

    close(fd);

  }

  return 0;
}
