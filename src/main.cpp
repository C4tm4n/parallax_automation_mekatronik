#include <Servo.h> 
#include <arduino.h> 
#include <cppQueue.h>
#include "drive.h"
#include "constants.h"
#include "debug.h"
#include "mapping.h"






double time; 
bool first = true;
bool finished = false;
bool drivebackward = false;
bool driveLeft = false;
bool driveRight = false;
bool stop = false;
double lastStop;
double turnTime = 0.5;







void setup(){
    Serial.begin(9600);
    left.servo.attach(10);
    right.servo.attach(11);

    left.sensorPin = 5;
    right.sensorPin = 7;

    pinMode(left.sensorPin, INPUT);
    pinMode(right.sensorPin, INPUT);



    
    left.currentSpeed = 0;
    left.targetSpeed = 0;
    right.currentSpeed = 0;
    right.targetSpeed = 0;
    left.sensorTrigered = false; 
    right.sensorTrigered = false;

}



void loop()
{
    readSensors();
    ct ++; 
    if(first){
        time = micros()/1000000.0;
        first = false;
    }
    else{
        looptime = micros()/1000000.0 -time;
        time = micros()/1000000.0;
    
    }
    evaluateSensorReadings();

            

    updateSpeeds();
    debug();
    estimateMovement();
}