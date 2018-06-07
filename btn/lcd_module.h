#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>

#define LCD_RS 27
#define LCD_E 28
#define LCD_D4 6
#define LCD_D5 5
#define LCD_D6 4
#define LCD_D7 1

int lcd_init();
void write_lcd(int lcd, char* str);
