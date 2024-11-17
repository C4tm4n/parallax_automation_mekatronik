#ifndef DRIVE_H
#define DRIVE_H
#include "variables.h"
#include <structs.h>
void drive(float leftSpeed, float rightSpeed);
float calculateSpeedDelta(struct motor servo);
void updateSpeeds();
void drive2(int absoluteX, int absoluteY, bool backward = false);
#endif
