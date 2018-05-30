#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>
#include <wiringPi.h>
#include <mcp3004.h>

#define DEV_PATH "/dev/btn_dev"
#define FRC_NUM 10

int find_median(int* arr, int size);
int main(int argc, char *argv[]) {
  int fd, btn_terminate = 0;
  int frc_val = 0;
  int frc_val_arr[FRC_NUM];
  int i = 0;

  wiringPiSetup();
  mcp3004Setup(300,0);

  while(1) {
    if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
      perror("open()");
      exit(1);
    }

    for (i=0; i<FRC_NUM; i++) {
      frc_val = analogRead(300);
      
      frc_val_arr[i] = frc_val;
      delay(500);
    }
  
    for (i=0; i<FRC_NUM; i++) {
      printf("%d\n",frc_val_arr[i]);
    }
    
    printf("frc median value :: %d \n", find_median(frc_val_arr, FRC_NUM));

    printf("continue??? 0 or anything :: ");
    scanf(" %d", &btn_terminate);
    
    if(btn_terminate==0)
	break;    

    close(fd);

  }

  return 0;
}
int find_median(int* arr, int size) {
  
  int tmp, i, j;
  
  for (i=0; i<size-1; i++) {
    for (j=i+1; j<size; j++) {
      if (arr[j] < arr[i]) {
        tmp = arr[i];
	arr[i] = arr[j];
        arr[j] = tmp;	
      }	      
    }
  }

  if (size%2==0) 
    return (arr[size/2] + arr[size/2 - 1]) /2;
  else
    return arr[size/2];	  

}
