#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define trigger 0
#define echo 1
#define HIGH 1
#define LOW 0

void wiringSetup();
void pinSet();
float getDistance();
void siren();
