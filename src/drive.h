#ifndef DRIVE_H
#define DRIVE_H
#include "variables.h"
#include <structs.h>
void drive(double leftSpeed, double rightSpeed);
double calculateSpeedDelta(struct motor servo);
void updateSpeeds();
bool drive2(double absoluteX, double absoluteY, bool backward = false);
bool rotate(double targetRotation);
#endif
