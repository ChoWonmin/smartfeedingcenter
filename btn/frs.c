#include "frs.h"

int wiringSetup(){
	if(wiringPiSetup()<0)
		return -1;
	else return 0;
}

int adcSetup(int pinBase, int spiNum){
	if(mcp3004Setup(pinBase,spiNum)<0)
		return -1;
	else
		return 0;
}

void readValue(int val_arr[FRC_NUM],int pinBase) {
	for(int i=0;i<FRC_NUM;i++){
		val_arr[i]=analogRead(pinBase);
		delay(100);
	}
}

int find_median(int* arr, int size) {
	int tmp, i, j;
	for (i=0; i<size-1; i++) {
		for (j=i+1; j<size; j++) {
			if (arr[j] < arr[i]) {
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
	
	if (size%2==0)
		return (arr[size/2] + arr[size/2 - 1]) /2;
	else
		return arr[size/2];
}

float getWeight(int* arr, int pinBase, int size){

	readValue(arr,pinBase);
	int median = find_median(arr,size);
	float voltage= (float)median/1023*3.3;

	if(0<=voltage&&voltage<1)
		return voltage*40;
	else if(1<=voltage&&voltage<1.5)
		return (voltage-2/3)*120;
	else if(1.5<=voltage&&voltage<2.5)
		return (voltage-1.3)*500;
	else if(2.5<=voltage&&voltage<3)
		return (voltage-1.75)*800;
	else
		return -1;
}
