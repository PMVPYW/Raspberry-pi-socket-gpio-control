#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <pthread.h>
#include "lights.h"
#include "misc.h"


int main() {
    wiringPiSetupGpio();
    for (int i = 0; i < 8; i++)
        pinMode(i+1, OUTPUT);
    int light_masks[LIGHTS_NUMBER];
    int light = 0xf;
    checkLights();
    createMasks(light_masks);
    printBin(light);
    turnOn(&light, 1, light_masks);
    update(light, light_masks);
    sleep(1);
    turnOff(&light, 1, light_masks);
    update(light, light_masks);
    printf("\n");
    printBin(light);
    printf("\n");
    return 0;
}



