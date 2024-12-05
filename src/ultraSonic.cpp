#include <arduino.h> 
#include "ultraSonic.h"
#include "debug.h"


unsigned long pingTime(int pingPin){
    //returnerar tiden i microsekunder
pinMode(pingPin, OUTPUT);
digitalWrite(pingPin, LOW);
delayMicroseconds(2);
digitalWrite(pingPin, HIGH);
delayMicroseconds(5);
digitalWrite(pingPin, LOW);
pinMode(pingPin, INPUT);
unsigned long duration = pulseIn(pingPin, HIGH);

debug("duration", duration,500);
return duration;
}

float micro2m(unsigned long microseconds){
    float meters = 343 * microseconds * pow(10,-6)/2;
    return meters;
}

float filter(float oldValue, float newReading, float weight){
    return oldValue*(1-weight) + newReading*weight;
}


float readDistanceSensor(int pingPin){
    return micro2m(pingTime(pingPin));
}


