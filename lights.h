#include <stdlib.h>
//include wiringpi

#ifndef LIGHTS_H_INCLUDED
#define LIGHTS_H_INCLUDED

#define LIGHTS_NUMBER 8

void createMasks(int *reference);
int isOn(int light, int light_id, int *masks);
void turnOn(int *light, int light_id, int *masks);
void turnOff(int *light, int light_id, int *masks);
void checkLights();
void update(int light, int *reference);

#endif // LIGHTS_H_INCLUDED
