#include <arduino.h> 
void readSensors();
void calculatePosition(double movement);
void estimateMovement();
void evaluateSensorReadings();
void performAction();
void estimateRotation(double diff);
void checkObstaclesClose(double x, double y);