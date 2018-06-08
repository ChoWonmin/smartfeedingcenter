#include "echoM.h"

void wiringSetup(){
    if(wiringPiSetup()<0){
        printf("wiringPi setup failing!");
        return;
    }
    else return;
}

void pinSet(){
    pinMode(trigger,OUTPUT);
    pinMode(echo,INPUT);
}

float getDistance(){
    float dist,s,e;

        digitalWrite(trigger,LOW);
        digitalWrite(trigger,HIGH);
        delay(100);
        digitalWrite(trigger,LOW);

        while(digitalRead(HIGH)==trigger)
            s=micros();
        while(digitalRead(HIGH)==echo)
            e=micros();

        dist= (e-s)*17/1000;
        return dist;
}

void siren(){
    system("omxplayer siren.mp3");
}
