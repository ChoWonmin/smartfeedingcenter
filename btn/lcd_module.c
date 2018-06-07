#include "lcd_module.h"

int lcd_init()
{	
	int lcd;

	if(lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0)){
		perror("lcd init failed!\n");
		return -1;
	}
	lcdClear(lcd);

	return lcd;
}
void write_lcd(int lcd, char* str) {
	lcdClear(lcd);
	lcdPosition(lcd,0,0);
	lcdPuts(lcd, str);
}
