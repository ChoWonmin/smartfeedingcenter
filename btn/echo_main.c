
#include "echoM.h"

static flag =0;

int main(void){

    float dist;

    wiringSetup();
    pinSet(trigger,echo);

    while(1){

        dist = getDistance(trigger,echo);

        if(dist<40)
            flag++;
        else
            flag=0;

        printf("distance : %.2f cm\n",dist);
        if(flag>4)
            siren();
        delay(100);
    }

    return 0;
}
