#ifndef __BUZZER_H
#define __BUZZER_H

#define CHARMAX 100
#define BUFF_SIZE 1024
#define DEV_PATH "/dev/buzzer_v"

void buzzer_work();
void *check_system_time(void* argv);

#endif 
