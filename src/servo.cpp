#include <arduino.h> 
#include <Servo.h> 
void updateSpeeds();

struct motor{

    Servo servo;
    double currentSpeed;
    double targetSpeed;
};
struct motor left;
struct motor right;

double acceleration = 0.21; 
double tMaxSpeed = 0.2;
double pMaxSpeed = 0.15;

double looptime = 0.001;
double time; 
bool first = true;
unsigned int ct;
bool finished = false;

void setup(){
    Serial.begin(9600);
    left.servo.attach(10);
    right.servo.attach(11);
    
    left.currentSpeed = 0;
    left.targetSpeed = 0;
    right.currentSpeed = 0;
    right.targetSpeed = 0;
}

void rightWheel(float speed){
    float microSeconds = 1498-592.32*speed;
    right.servo.writeMicroseconds(microSeconds);
}
void leftWheel(float speed){
    left.servo.writeMicroseconds(1496.8+ 586.71*speed);

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
        left.targetSpeed = 0.15;
        right.targetSpeed = 0.15;
        if(ct%1000 == 0){
            Serial.println(time);}
    }else if(time < 9){
        if(ct%1000 == 0){
            Serial.println(time);}
        left.targetSpeed = 0.1;
        right.targetSpeed = 0;
    }else if(time < 12){
        if(ct%1000 == 0){
            Serial.println(time);}
        left.targetSpeed = 0;
        right.targetSpeed = 0.1;
    }else if(time < 15){
        left.targetSpeed = 0.15;
        right.targetSpeed = 0.15;
        if(ct%1000 == 0){
            Serial.println(time);}
    }else if(time < 22){
        left.targetSpeed = -0.15;
        right.targetSpeed = -0.15;
        if(ct%1000 == 0){
            Serial.println(time);}
    }else{
        left.targetSpeed = 0;
        right.targetSpeed = 0;
        if(ct%1000 == 0){
            Serial.println(time);
            
        }
        finished = true;
    }

    updateSpeeds();
}



float calculateSpeedDelta(struct motor servo){
    float currentAcceleration;
    currentAcceleration = acceleration*(tMaxSpeed - servo.currentSpeed);
    float speedDelta = currentAcceleration * looptime;

    return speedDelta;

}
void updateSpeeds(){
    float speedDelta;
    struct motor structArray [] = {left, right};
    for (struct motor side :structArray){
        speedDelta = calculateSpeedDelta(side);
        if(side.targetSpeed > side.currentSpeed){
            if(ct%1000 == 0){
                Serial.print("targetspeed: ");
                Serial.println(side.targetSpeed);
            }
        side.currentSpeed += speedDelta; 
        if(side.targetSpeed<side.currentSpeed){ // if the new speed goes above the target speed
            side.currentSpeed = left.targetSpeed; 
            }
        }
        else{
            side.currentSpeed -= speedDelta; 
            if(side.targetSpeed > side.currentSpeed){
                side.currentSpeed = side.targetSpeed;
            }
        }
    }

    rightWheel(right.currentSpeed);
    leftWheel(left.currentSpeed);
    if(ct%1000 == 0){
        Serial.print("left: ");
        Serial.println(left.currentSpeed);
        Serial.print("right: ");
        Serial.println(right.currentSpeed);
    }
}