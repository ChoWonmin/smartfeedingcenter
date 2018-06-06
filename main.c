#include "frs.h"
int main(void){

    int val_arr[FRC_NUM];
    int median;
    float weight;

    wiringPiSetup();

    adcSetup(300,0);
    readValue(val_arr,300);
    median = find_median(val_arr,FRC_NUM);

    weight =  getWeight(median);
    printf("%f\n",weight);


    return 0;
}
