#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>

#define DEV_PATH "/dev/btn_dev"

int main(int argc, char *argv[]) {
    int fd = 0;
    
    if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
        perror("open()");
        exit(1);
    }
    
    printf("open successs!");
    sleep(2);
    
    close(fd);
    
    return 0;
}
