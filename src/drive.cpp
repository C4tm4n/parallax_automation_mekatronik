#include <arduino.h> 
#include <Servo.h> 
#include "drive.h"
#include "variables.h"

double looptime = 0.0001;
struct motor left;
struct motor right;


void drive2(int absoluteX, int absoluteY, bool backward =false){
    double relativeX = absoluteX-xPos; //ignoring rotation
    double relativeY = absoluteY -yPos;

    if(abs(rotation) < PI/4){
        if(relativeX > 0){
            right.targetSpeed = min(relativeX/10*pMaxSpeed,pMaxSpeed);
            left.targetSpeed = min(relativeX/10*pMaxSpeed,pMaxSpeed);
        }
        else if(backward){
            right.targetSpeed = max(relativeX/10*pMaxSpeed,-pMaxSpeed);
            left.targetSpeed = max(relativeX/10*pMaxSpeed,-pMaxSpeed);
        }
    }
    else if(abs(rotation) < PI * 3/4){
        if(relativeY > 0){
            if(rotation >0){
                right.targetSpeed = min(relativeY/10*pMaxSpeed,pMaxSpeed);
                left.targetSpeed = min(relativeY/10*pMaxSpeed,pMaxSpeed);
            }
        }
        else if(backward){
            right.targetSpeed = max(relativeX/10*pMaxSpeed,-pMaxSpeed);
            left.targetSpeed = max(relativeX/10*pMaxSpeed,-pMaxSpeed);
        }
    }

}

void drive(double leftSpeed, double rightSpeed){
    left.servo.writeMicroseconds(1496.8- 586.71*leftSpeed);
    right.servo.writeMicroseconds(1498+592.32*rightSpeed);
}

double calculateSpeedDelta(struct motor servo){
    double currentAcceleration;
    //currentAcceleration = acceleration*(tMaxSpeed - servo.currentSpeed);
    if(abs(left.targetSpeed)>abs(left.currentSpeed)){
        currentAcceleration = acceleration;
    }
    else{
        currentAcceleration = breakAcceleration;
    }
    double speedDelta = currentAcceleration * looptime;

    return speedDelta;

}


void updateSpeeds(){
    double speedDelta;
    speedDelta = calculateSpeedDelta(left);
    if(left.targetSpeed > left.currentSpeed)
    {

        left.currentSpeed += speedDelta; 
        if(left.targetSpeed<left.currentSpeed)
        { // if the new speed goes above the target speed
        left.currentSpeed = left.targetSpeed; 
        }
    }
    else{
        left.currentSpeed -= speedDelta; 
        if(left.targetSpeed > left.currentSpeed){
            left.currentSpeed = left.targetSpeed;
            }
    }


    speedDelta = calculateSpeedDelta(right);
    if(right.targetSpeed > right.currentSpeed)
    {

        right.currentSpeed += speedDelta; 
        if(right.targetSpeed<right.currentSpeed)
        { // if the new speed goes above the target speed
        right.currentSpeed = right.targetSpeed; 
        }
    }
    else{
        right.currentSpeed -= speedDelta; 
        if(right.targetSpeed > right.currentSpeed){
            right.currentSpeed = right.targetSpeed;
            }
    }

    drive(left.currentSpeed, right.currentSpeed);
}