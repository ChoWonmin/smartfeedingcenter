#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>
#include <wiringPi.h>
#include <lcd.h>

#define LCD_RS 11
#define LCD_E 10
#define LCD_D4 6
#define LCD_D5 5
#define LCD_D6 4
#define LCD_D7 1

#define DEV_PATH "/dev/btn_dev"

int lcd_init(int lcd)
{
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

int main(int argc, char *argv[]) {
    int fd, repeat = 0;
    int on = 0;
    int lcd;
    
    lcd = lcd_init(lcd);
    
    while(1){
        if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
            perror("open()");
            exit(1);
        }
        close(fd);
        
        on = on?0:1;
        printf("on : %d\n",on);
        
        if(on) {
            
            write_lcd(lcd,"lcd turn on\n");
            
            //printf("Process 1: continue , 0: terminate :: \n");
            //scanf(" %d\n", &repeat);
            
            //if(!repeat)
            //    break;
        }else {
            printf("lcd turin off\n");
            write_lcd(lcd,"off \n");
        }
        
    }
    
    return 0;
}
