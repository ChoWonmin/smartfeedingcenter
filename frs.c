#include <stdio.h>
#include <wiringPi.h>
#include <mcp3004.h>
 
int main(){
 
        int val= 0;
 
        wiringPiSetup();
        mcp3004Setup(300,0);
 
        while(1)
        {
                val = analogRead(300);
 
                printf("%d\n",val);
                delay(500);
 
        }
 
        return 0;
}
