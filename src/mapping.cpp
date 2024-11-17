#include <Arduino.h>
#include <mapping.h>
#include "variables.h"
#include "debug.h"
struct obstacle
{
    int minX;
    int maxX;
    int minY;
    int maxY;
    int connected;
};
struct obstacle obstacles[maxObstacles];
int obstacleCt = 0;

int xPos = 0;
int yPos = 0;
int rotation = 0;

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
    int movement;
    int diff = left.targetSpeed -right.targetSpeed;
    if(diff >0){
        movement = left.targetSpeed * looptime;
    }
    else{
        movement = right.targetSpeed * looptime;
    }
    

    calculatePosition(movement);
    estimateRotation(diff);
}

void calculatePosition(int movement){
    xPos += movement* asin(rotation);
    yPos += movement* acos(rotation);
}

int relativ2absolute(int relativ, bool isX){
    if(isX){
        return xPos + relativ;
    }
    else{
        return yPos + relativ;
    }



}

void estimateRotation(int diff){
    int relativeRotation = diff*looptime/(WheelBase);
    rotation += relativeRotation;

//rotaion = singleWheelmovemnt /circlediameter * 2Pi


}


void mapping(){

}

void addObstacle(){
    int obstacleX = xPos + sensorOffset*acos(rotation) +sensorOffsetSide*asin(rotation);
    int obstacleY = xPos + sensorOffset*asin(rotation) +sensorOffsetSide*acos(rotation);
    obstacle newObstackle;
    newObstackle.maxX = obstacleX;
    newObstackle.minX = obstacleX;
    newObstackle.maxY = obstacleY;
    newObstackle.minY = obstacleY;

    obstacles[obstacleCt%maxObstacles] = newObstackle; 
}

void checkObstacles(){

    for (obstacle obstacle : obstacles){

    }
}

void readSensors(){
    left.sensorTrigered = digitalRead(left.sensorPin);
    right.sensorTrigered = digitalRead(right.sensorPin);
}

