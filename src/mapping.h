#include <arduino.h> 
void readSensors();
void calculatePosition(double movement);
void estimateMovement();
void evaluateSensorReadings();
void performAction();
void estimateRotation(double diff);
int checkObstaclesClose(double x, double y);
void foundObstacle(int side);
void addObstacle(double obstacleX, double obstacleY);
int checkForward();
int relativ2absolute(double relativ, bool isX);
