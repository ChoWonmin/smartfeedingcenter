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

#define BUFFER_LENGTH 256
#define DEV_PATH "/dev/btn_dev"

#define LCD_RS 27
#define LCD_E 28
#define LCD_D4 6
#define LCD_D5 5
#define LCD_D6 4
#define LCD_D7 1

static char receive[BUFFER_LENGTH];

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
    printf("your str :: %s\n", str);
    
    lcdPosition(lcd,0,0);
    lcdPuts(lcd, str);
}

int main(){
    int ret, fd;
    char stringToSend[BUFFER_LENGTH];
    int on = 0;
    
    int lcd;
    
    lcd = lcd_init();
    
    if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
        perror("open()");
        exit(1);
    }
    
    while(1){
        ret = read(fd, receive, BUFFER_LENGTH);
        
        if (ret < 0) {
            perror("Failed to read... \n");
            return errno;
        }
        
        if (strcmp(receive,"click")==0)
            on = on?0:1;
        
        if (on)
            write_lcd(lcd,"on");
        else
            write_lcd(lcd,"        ");
        
        sleep(1);
    }
    
    close(fd);
    
    return 0;
}

