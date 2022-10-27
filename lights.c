#include <wiringPi.h>
#include "lights.h"

void checkLights() {
    int int_size = sizeof (int) * 8;
    if (LIGHTS_NUMBER > int_size) {
        exit(1);
    }
}

void turnOff(int *light, int light_id, int *masks) {
    *light = *light & ~masks[light_id];
}

void turnOn(int *light, int light_id, int *masks) {
    *light = *light | masks[light_id];
}

int isOn(int light, int light_id, int *masks) {
    int i = light & masks[light_id];
    if (i != 0) {
        i = 1;
    }
    return i;
}


void createMasks(int *reference) {
    for (int i = 0; i < LIGHTS_NUMBER; i++) {
        reference[i] = 1 << i;
    }
}

void update(int light, int *reference) {
    for (int i = 0; i < LIGHTS_NUMBER; i++)
    {
        digitalWrite(i+1, isOn(light, i, reference));
    }
    
}
