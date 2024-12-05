#include <Arduino.h>
#include <mapping.h>
#include "variables.h"
#include "debug.h"
#include "drive.h"
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
target targets[targetCT];
int currentTarget;
int lastTarget;

turns currentAction;
turns nextAction;
enum sensorReading currentReading = CLEAR;

void performAction(){

    if(stop){
        left.targetSpeed = 0;
        right.targetSpeed = 0;
        if(abs(left.currentSpeed) < 0.1 && abs(right.currentSpeed) < 0.1){
            stop = false;
            lastStop = time;
        }
        
    }
    else{
        //target* ptr = targets[currentTarget];
        if(targets[currentTarget].isX){
            int direction;
            if(targets[currentTarget].x >xPos){
                direction = 1;
            }
            else{
                direction = -1;
            }
            if(rotate(direction*PI/2)){
                debug("drive x",2000);
                debug(String(targets[currentTarget].y),2000);
                debug(String(targets[currentTarget].x),2000);
                if(drive2(targets[currentTarget].x,targets[currentTarget].y)){
                    currentTarget++;
                }
            }

        }
        else{
            int direction;
            if(targets[currentTarget].y >yPos){
                direction = 1;
            }
            else{
                direction = -1;
            }
            if(rotate(0)){
                debug("drive y",2000);
                debug(String(targets[currentTarget].y),2000);
                debug(String(targets[currentTarget].x),2000);
                if(drive2(targets[currentTarget].x,targets[currentTarget].y)){
                    currentTarget++;
                }
            }
        
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
        foundObstacle(1);

        if(nextAction == NONE ){
            currentAction = BACKWARD;
            nextAction = LEFT;
        }
    }
    else if(right.sensorTrigered == 1) //if left sensor trigered
    {
        currentReading = SLEFT;
        foundObstacle(-1);
        if(nextAction == NONE ){
            currentAction = BACKWARD;
            nextAction = RIGHT;
        }
    }
    else //both sensors triggered
    {

        currentReading = BOTH;
        foundObstacle(0);
        if(nextAction != EVALUATE ){
            currentAction = BACKWARD;
            nextAction = EVALUATE;
        }
    }

}


void estimateMovement(){
    double movement;
    double diff = (left.currentSpeed -right.currentSpeed);
    if(diff >0){
        movement = (left.currentSpeed -diff/2)* looptime;
    }
    else{
        movement = (right.currentSpeed -diff/2)* looptime;
    
    }
    

    calculatePosition(movement);
    estimateRotation(diff);
}

void calculatePosition(double movement){
    if(isnan(movement)){
        debug("nan detected movement",1);
        debug(String(movement),1);
    }
    debug(String(movement),2000);
    double change = movement* asin(rotation);
    if(isnan(change)){
        change = 0;
    }
    xPos += change;
    change = movement* acos(rotation);
    if(isnan(change)){
        change = 0;
    }
    yPos += change;
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
    int forward = checkForward();
    int oRight = checkObstaclesClose(xPos + 0.1, yPos+0.1*(forward) );
    int oLeft = checkObstaclesClose(xPos - 0.1, yPos+0.1*(forward) );


}

int checkForward(){
    int i;
    for(i = 0; i<targetCT; i++){
        int state = checkObstaclesClose(xPos, yPos +0.1* i);
        if(state == -1){
            continue;
        }
        else{
            i--;
            break;
        }
    }
    return i;
}




void foundObstacle(int side){
    double obstacleX = xPos + sensorOffset*acos(rotation) +sensorOffsetSide*asin(rotation) *side;
    double obstacleY = xPos + sensorOffset*asin(rotation) +sensorOffsetSide*acos(rotation)*side;

    int close = checkObstaclesClose(obstacleX,obstacleY);
    //if(close == -1){
    if(true){
        addObstacle(obstacleX, obstacleY);
    }
    else{
        obstacle* ptr;
        ptr = &obstacles[close];
        if((*ptr).X >obstacleX){
            ptr->X = obstacleX;
        }
        else if((*ptr).X +(*ptr).width<obstacleX){
            ptr->width = obstacleX - (*ptr).X;
        }
        if((*ptr).Y >obstacleY){
            ptr->Y = obstacleY;
        }
        else if((*ptr).Y +(*ptr).height<obstacleY){
            ptr->height = obstacleY - (*ptr).Y;
        }
    }


}

void addObstacle(double obstacleX, double obstacleY){
    obstacle newObstackle;
    newObstackle.X = obstacleX;
    newObstackle.Y = obstacleY;
    newObstackle.width = 0.01;
    newObstackle.height = 0.01;

    obstacles[obstacleCt%maxObstacles] = newObstackle; 
}

int checkObstaclesClose(double x, double y){
    double xDistance;
    double yDistance;
    for (int i = 0; i<obstacleCt%maxObstacles; i++){
        obstacle obs = obstacles[i];

        yDistance = y - obs.Y;
        if(yDistance >-0.1 && yDistance < (obs.height +0.1)){
            xDistance = x - obs.X;
            if(xDistance >-0.1 && xDistance < (obs.width +0.1)){
                return i;
                
            }

        }

    }
    return -1; 
}

void readSensors(){
    left.sensorTrigered = digitalRead(left.sensorPin);
    right.sensorTrigered = digitalRead(right.sensorPin);
}

