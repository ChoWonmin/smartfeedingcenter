#include <stdio.h>
#include <wiringPi.h>
#include <mcp3004.h>
 
int main(){
 
        int val= 0;
 
        wiringPiSetup();
        mcp3004Setup(300,0);
 
        for(;;)
        {
                val = analogRead(300);
 
                printf("ch1:%d\n",val);
                delay(500);
 
        }
 
        return 0;
}
