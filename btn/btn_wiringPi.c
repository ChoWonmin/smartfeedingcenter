#include <stdio.h>
#include <wiringPi.h>

void scanButton (int button)
{
    if (digitalRead (button) == HIGH)    // Low is pushed
        return ;
    
    printf("click\n");
    
    while (digitalRead (button) == LOW)    // Wait for release
        delay (10) ;
}

int main (void)
{
    int num = 8;
    
    printf("start \n");
    
    wiringPiSetup () ;
    
    while(1){
        scanButton (num);
    }
    
}
