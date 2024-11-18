#ifndef VARIABLES_H
#define VARIABLES_H
#include <Servo.h>
#define WheelBase 0.107 
#define sensorOffset 0.06
#define sensorOffsetSide 0.08
#define acceleration 0.2
#define breakAcceleration 1
#define tMaxSpeed 0.2
#define pMaxSpeed 0.15
#define maxRotationSpeed 0.1
#define maxObstacles 30
#define targetCT 10

extern double looptime;
extern double xPos;
extern double yPos;
extern double rotation;
extern unsigned int ct;
extern double time;
extern bool stop;
extern double lastStop;

struct motor{
    Servo servo;
    double currentSpeed;
    double targetSpeed;
    int sensorPin;
    int sensorTrigered;
};
extern struct motor left;
extern struct motor right;

struct target{
    double x;
    double y;
    double rotation;
};
extern target targets[targetCT];
extern int currentTarget;
extern int lastTarget;

enum turns {
    RIGHT,
    LEFT,
    NONE,
    UTURN,
    BACKWARD,
    STOP,
    EVALUATE
};

extern turns currentAction;
extern turns nextAction;
enum sensorReading{
    BOTH,
    CLEAR,
    SLEFT,
    SRIGHT
};
extern enum sensorReading currentReading;
#endif