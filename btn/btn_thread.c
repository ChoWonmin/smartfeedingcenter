#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/kdev_t.h>
#include <wiringPi.h>
#include <lcd.h>
#include <pthread.h>

#define BUFFER_LENGTH 256
#define DEV_PATH "/dev/btn_dev"

#define LCD_RS 28
#define LCD_E 27
#define LCD_D4 6
#define LCD_D5 5
#define LCD_D6 4
#define LCD_D7 1

static char receive[BUFFER_LENGTH];
int on = 0;

int lcd_init();
void write_lcd(int lcd, char* str);
void *btn_scan(void *data) {
    int ret, fd;
    
    pid_t pid;
    pthread_t tid;
    
    if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
        perror("open()");
        exit(1);
    }
    
    while(1) {
        ret = read(fd, receive, BUFFER_LENGTH);
        
        if (ret < 0) {
            perror("Failed to read... \n");
            exit(1);
        }
        
        if (strcmp(receive,"click")==0)
            on = on?0:1;
        
        sleep(1);
    }
    
    close(fd);
}

int main(){
    pthread_t p_thread[2];
    int thr_id;
    int status;
    char p1[] = "btn_scan";
    char pM[] = "thread_m";
    
    thr_id = pthread_create(&p_thread[0], NULL, btn_scan, (void *)p1);
    
    if (thr_id < 0) {
        perror("Button scan tread create error");
        exit(0);
    }
    
    int lcd;
    
    lcd = lcd_init();
    
    while(1){
        if (on)
            write_lcd(lcd,"on");
        else
            write_lcd(lcd,"        ");
        
        sleep(1);
    }
    
    pthread_join(p_thread[0], (void **)&status);
    
    return 0;
}
int lcd_init()
{
    int lcd;
    
    wiringPiSetup();
    
    if(lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0)){
        perror("lcd init failed!\n");
        return -1;
    }
    
    return lcd;
}
void write_lcd(int lcd, char* str){
    lcdPosition(lcd,0,0);
    lcdPuts(lcd, str);
}
