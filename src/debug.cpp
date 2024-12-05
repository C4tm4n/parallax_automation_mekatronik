#include <arduino.h> 
#include "variables.h"
#include "debug.h"
unsigned int ct;
void debug(){
    if(ct%500 == 0){
        debug("time",time,1);
//        Serial.print("sensors r + l:");
//        Serial.print(right.sensorTrigered);
//        Serial.println(left.sensorTrigered);
        Serial.print("left targetspeed: ");
        Serial.println(left.targetSpeed,4);
        Serial.println(left.currentSpeed);
        Serial.print("right targetspeed: ");
        Serial.println(right.targetSpeed,4);
        Serial.println(right.currentSpeed);
//        Serial.print("looptime: ");
//        Serial.println(looptime,6);
        Serial.println("position");
        Serial.println(xPos,3);
        Serial.println(yPos,3);
        Serial.println(rotation,3);
        printStruct(currentTargetStruct,"target2");
    }

}
void debug(String msg, int freq){
    if(ct%freq == 0){
        Serial.println(msg);
    }
}
void debug(String msg,double value, int freq){
    if(ct%freq == 0){
        Serial.print(msg + ": ");
        Serial.println(value);
    }
}

void printStruct(struct target position, String name, int freq = 500){
    if(ct%freq == 0){
        Serial.println(name);
        Serial.println(position.x);
        Serial.println(position.y);
    }
}