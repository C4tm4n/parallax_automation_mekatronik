#include <arduino.h> 
#include "variables.h"
unsigned int ct;
void debug(){
    if(ct%2000 == 0){
        Serial.print("sensors r + l:");
        Serial.print(right.sensorTrigered);
        Serial.println(left.sensorTrigered);
        Serial.print("left targetspeed: ");
        Serial.println(left.targetSpeed);
        Serial.print("currentspped: ");
        Serial.println(left.currentSpeed);
        Serial.print("right targetspeed: ");
        Serial.println(right.targetSpeed);
        Serial.print("currentspped: ");
        Serial.println(right.currentSpeed);
        Serial.println(left.currentSpeed);
        Serial.print("actions");
        Serial.println(currentAction);
        Serial.println(nextAction);
        Serial.print("looptime: ");
        Serial.println(looptime,6);
        Serial.println(time,6);
        Serial.println(lastStop,6);
    }

}
void debug(String msg, int freq){
    if(ct%freq == 0){
        Serial.println(msg);
    }
}