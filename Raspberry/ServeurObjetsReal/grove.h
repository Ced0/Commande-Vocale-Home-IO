#ifndef GROVE_H
#define GROVE_H

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>


void init();

float temperature();

void relay(bool etat);

float humidity();

#endif // GROVE_H
