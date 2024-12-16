#include <Servo.h> 
#include <arduino.h> 
#include <cppQueue.h>
#include "drive.h"
#include "variables.h"
#include "debug.h"
#include "ultraSonic.h"
#include "mapping.h"



double time; 
bool first = true;
bool stop = false;
double lastStop;
int ultraSonicPin = 12;
float distance;

struct target currentTargetStruct;



void setup(){
    Serial.begin(9600);
    left.servo.attach(10);
    right.servo.attach(11);



    
    left.currentSpeed = 0;
    left.targetSpeed = 0;
    right.currentSpeed = 0;
    right.targetSpeed = 0;

}



void loop()
{
    float newDistance = readDistanceSensor(ultraSonicPin);
    debug("reading", newDistance,500);
    ct ++; 
    if(first){
        time = micros()/1000000.0;
        first = false;
        distance = newDistance;
    }
    else{
        looptime = micros()/1000000.0 -time;
        time = micros()/1000000.0;
    
    }
    distance = filter(distance, newDistance, 0.1);
    currentTargetStruct.isX = false;
    currentTargetStruct.y = yPos + distance -0.2;
    currentTargetStruct.x = xPos; 
    printStruct(currentTargetStruct, "pos");
    debug("distance", distance,500);
    drive2(currentTargetStruct.x, currentTargetStruct.y);

    





    updateSpeeds();
    debug();

}