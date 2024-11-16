#ifndef STRUCTS_H
#define STRUCTS_H
#include <arduino.h> 
#include <Servo.h> 

struct motor{

    Servo servo;
    double currentSpeed;
    double targetSpeed;
    int sensorPin;
    int sensorTrigered;
};
#endif