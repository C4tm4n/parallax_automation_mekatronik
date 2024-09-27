#include <arduino.h> 
#include <Servo.h> 
void updateSpeeds();

Servo ServoLeft;
Servo ServoRight;
double currentSpeedRight = 0;
double currentSpeedLeft = 0;
double acceleration = 0.42; 
double maxSpeed = 0.2;

double targetSpeedRight;
double targetSpeedLeft;

double looptime = 0.001;
double time; 
bool first = true;
unsigned int ct;
bool finished = false;

void setup(){
    ServoLeft.attach(10);
    ServoRight.attach(11);
    Serial.begin(9600);
}

void rightWheel(float speed){

    ServoRight.writeMicroseconds(1500-592.32*speed);
}
void leftWheel(float speed){
    ServoLeft.writeMicroseconds(1500+ 586.71*speed);

}

void loop()
{
    ct ++; 
    if(first){
        time = micros()/1000000.0;
        first = false;
    }
    else{
        looptime = micros()/1000000.0 -time;
        time = micros()/1000000.0;
    }
    if(finished){

    }
    else if(time< 4){
        targetSpeedLeft = 0.15;
        targetSpeedRight = 0.15;
        if(ct%1000 == 0){
            Serial.println(time);}
    }else if(time < 9){
        if(ct%1000 == 0){
            Serial.println(time);}
        targetSpeedLeft = 0.1;
        targetSpeedRight = 0;
    }else if(time < 12){
        if(ct%1000 == 0){
            Serial.println(time);}
        targetSpeedLeft = 0;
        targetSpeedRight = 0.1;
    }else if(time < 15){
        targetSpeedLeft = 0.15;
        targetSpeedRight = 0.15;
        if(ct%1000 == 0){
            Serial.println(time);}
    }else if(time < 22){
        targetSpeedLeft = -0.15;
        targetSpeedRight = -0.15;
        if(ct%1000 == 0){
            Serial.println(time);}
    }else{
        targetSpeedLeft = 0;
        targetSpeedRight = 0;
        if(ct%1000 == 0){
            Serial.println(time);
            
        }
        finished = true;
    }

    updateSpeeds();
}



float calculateSpeedDelta(bool rightMotor){
    float currentSpeed;
    float currentAcceleration;
    float targetSpeed;
    if(rightMotor){
        currentSpeed = currentSpeedRight;
        targetSpeed = targetSpeedRight;

    }
    else{
        currentSpeed = currentSpeedLeft;
        targetSpeed = targetSpeedLeft;
    }
    currentAcceleration = acceleration*(maxSpeed - currentSpeed);
    float speedDelta = currentAcceleration * looptime;

    return speedDelta;

}
void updateSpeeds(){
    float speedDelta;
    
    speedDelta = calculateSpeedDelta(true);
    if(targetSpeedRight > currentSpeedRight){
    currentSpeedRight += speedDelta; 
    }
    else{
    currentSpeedRight -= speedDelta; 
    }

    speedDelta = calculateSpeedDelta(false);
    if(targetSpeedLeft > currentSpeedLeft){
    currentSpeedLeft += speedDelta; 
    }
    else{
    currentSpeedLeft -= speedDelta; 

    }

    rightWheel(currentSpeedRight);
    leftWheel(currentSpeedLeft);
    if(ct%1000 == 0){
        Serial.print("left: ");
        Serial.println(currentSpeedLeft);
        Serial.print("right: ");
        Serial.println(currentSpeedRight);
    }
}