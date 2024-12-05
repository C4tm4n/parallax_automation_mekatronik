#include <arduino.h> 
#include <Servo.h> 
#include "drive.h"
#include "variables.h"
#include "debug.h"

double looptime = 0.0001;
struct motor left;
struct motor right;



bool rotate(double targetRotation){
    double deltaRotation = targetRotation -rotation;
    if(deltaRotation >PI){
        deltaRotation -= 2*PI;
    }
    else if(deltaRotation<-2*PI){
        deltaRotation += 2*PI;
    }

    debug(String(deltaRotation), 2000);
    double rotationSpeed = min(maxRotationSpeed*abs(deltaRotation)/PI*4,maxRotationSpeed);
    if(deltaRotation>0){
        right.targetSpeed = rotationSpeed;
        left.targetSpeed = -rotationSpeed;
    }
    else{
        right.targetSpeed =-rotationSpeed;
        left.targetSpeed = rotationSpeed;
    }
    if(abs(deltaRotation) < 0.05){
        return true;
    }
    else{
        return false;
    }

}


bool drive2(double absoluteX, double absoluteY, bool backward =false){
    double relativeX = absoluteX-xPos; //ignoring rotation
    double relativeY = absoluteY -yPos;
    debug("relativ",2000);
    debug(String(relativeX), 2000);
    debug(String(relativeY), 2000);
    if(abs(relativeX)<0.01 && abs(relativeY) <0.01){
        debug("arrived",1000);
        right.targetSpeed= 0;
        left.targetSpeed= 0;
        return true;
    }

    if(abs(rotation) < PI/4){
        if(relativeY > 0){
            right.targetSpeed = min(relativeY/breakingDistance*pMaxSpeed,pMaxSpeed);
            left.targetSpeed = min(relativeY/breakingDistance*pMaxSpeed,pMaxSpeed);
        }
        else {
            right.targetSpeed = max((relativeY/breakingDistance)*pMaxSpeed,-pMaxSpeed);
            left.targetSpeed = max((relativeY/breakingDistance)*pMaxSpeed,-pMaxSpeed);
        }
        debug("speed",2000);
        debug(String(left.targetSpeed),2000);
        debug(String(right.targetSpeed),2000);
    }
    else if(abs(rotation) < PI * 3/4){
        if(relativeX > 0){
            if(rotation >0){
                right.targetSpeed = min((relativeX/breakingDistance)*pMaxSpeed,pMaxSpeed);
                left.targetSpeed = min((relativeX/breakingDistance)*pMaxSpeed,pMaxSpeed);
            }
        }
        else if(backward){
            right.targetSpeed = max(relativeX/breakingDistance*pMaxSpeed,-pMaxSpeed);
            left.targetSpeed = max(relativeX/breakingDistance*pMaxSpeed,-pMaxSpeed);
        }
    }
    return false;

}

void drive(double leftSpeed, double rightSpeed){
    left.servo.writeMicroseconds(1496.8- 586.71*leftSpeed);
    right.servo.writeMicroseconds(1498+592.32*rightSpeed);
}

double calculateSpeedDelta(struct motor servo){
    double currentAcceleration;
    //currentAcceleration = acceleration*(tMaxSpeed - servo.currentSpeed);
    if(abs(left.targetSpeed)<abs(left.currentSpeed)){
        currentAcceleration = breakAcceleration;
    }
    else{
        currentAcceleration = acceleration;
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