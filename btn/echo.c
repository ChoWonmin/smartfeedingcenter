
#include <stdio.h>
#include<stdlib.h>
#include <wiringPi.h>

static int flag=0;

int main(void){
float dist, s, e;
wiringPiSetup();
pinMode(0,OUTPUT);
pinMode(1,INPUT);

while(1){
digitalWrite(0,0);
digitalWrite(0,1);
delay(100);
digitalWrite(0,0);

while(digitalRead(1)==0)
s=micros();

while(digitalRead(1)==1)
e=micros();

dist = (e-s)/58;

if(dist<40)
    flag++;
else
    flag =0;
printf("distance : %f cm \n",dist);
if(flag>4)
    system("omxplayer siren.mp3");
 delay(100);
}
return 0;
}
