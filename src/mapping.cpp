#include <Arduino.h>
#include <mapping.h>
#include "variables.h"
#include "debug.h"
struct obstacle
{
    int X;
    int Y;
    int width;
    int height;
    int connected;
};
struct obstacle obstacles[maxObstacles];
int obstacleCt = 0;

double xPos = 0;
double yPos = 0;
double rotation = 0;

turns currentAction;
turns nextAction;
enum sensorReading currentReading = CLEAR;

void performAction(){

    if(stop){
        left.targetSpeed = 0;
        right.targetSpeed = 0;
        if(left.currentSpeed == 0 && right.currentSpeed == 0){
            stop = false;
            lastStop = time;
        }
        
    }

}

void evaluateSensorReadings(){
    if(left.sensorTrigered == 1 && right.sensorTrigered == 1){ //no sensor triggered 
        currentReading = CLEAR;
    }
    else if(left.sensorTrigered == 1) //if right sensor trigered
    {
        currentReading = SRIGHT;
        obstacle();
        if(nextAction == NONE ){
            currentAction = BACKWARD;
            nextAction = LEFT;
        }
    }
    else if(right.sensorTrigered == 1) //if left sensor trigered
    {
        currentReading = SLEFT;
        obstacle();
        if(nextAction == NONE ){
            currentAction = BACKWARD;
            nextAction = RIGHT;
        }
    }
    else //both sensors triggered
    {

        currentReading = BOTH;
        obstacle();
        if(nextAction != EVALUATE ){
            currentAction = BACKWARD;
            nextAction = EVALUATE;
        }
    }

}

void estimateMovement(){
    double movement;
    double diff = (left.targetSpeed -right.targetSpeed);
    if(diff >0){
        movement = left.targetSpeed * looptime;
    }
    else{
        movement = right.targetSpeed * looptime;
    }
    

    calculatePosition(movement);
    estimateRotation(diff);
}

void calculatePosition(double movement){
    xPos += movement* asin(rotation);
    yPos += movement* acos(rotation);
}

int relativ2absolute(double relativ, bool isX){
    if(isX){
        return xPos + relativ;
    }
    else{
        return yPos + relativ;
    }



}

void estimateRotation(double diff){
    double relativeRotation = diff*looptime/(WheelBase);
    rotation += relativeRotation;

//rotaion = singleWheelmovemnt /circlediameter * 2Pi


}


void mapping(){

}

void addObstacle(){
    double obstacleX = xPos + sensorOffset*acos(rotation) +sensorOffsetSide*asin(rotation);
    double obstacleY = xPos + sensorOffset*asin(rotation) +sensorOffsetSide*acos(rotation);
    obstacle newObstackle;
    newObstackle.X = obstacleX;
    newObstackle.Y = obstacleY;
    newObstackle.width = 0.05;
    newObstackle.height = 0.05;

    obstacles[obstacleCt%maxObstacles] = newObstackle; 
}

void checkObstaclesClose(double x, double y){
    double xDistance;
    double yDistance;
    for (obstacle obs : obstacles){
        yDistance = y - obs.Y;
        if(yDistance >0.15){
            if(yDistance < y +0.15){

            }

        }
        xDistance = x - obs.X;
        if(xDistance >0){
            
        }

    }
}

void readSensors(){
    left.sensorTrigered = digitalRead(left.sensorPin);
    right.sensorTrigered = digitalRead(right.sensorPin);
}

