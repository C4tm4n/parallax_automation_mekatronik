#ifndef DRIVE_H
#define DRIVE_H
#include "variables.h"
#include <structs.h>
void drive(double leftSpeed, double rightSpeed);
double calculateSpeedDelta(struct motor servo);
void updateSpeeds();
void drive2(double absoluteX, double absoluteY, bool backward = false);
#endif
