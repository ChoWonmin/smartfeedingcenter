#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <mcp3004.h>

#define FRC_NUM 10

int wiringSetup();
int adcSetup(int pinBase, int spiNum);
void readValue(int val_arr[FRC_NUM],int pinBase);
int fine_median(int *arr,int size);
float getWeight(int *arr, int pinBase, int size);
