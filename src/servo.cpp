#include <arduino.h> 
#include <Servo.h> 
void updateSpeeds();

struct motor{

    Servo servo;
    double currentSpeed;
    double targetSpeed;
    int sensorPin;
    bool sensorTrigered;
};
struct motor left;
struct motor right;

double acceleration = 0.3; 
double tMaxSpeed = 0.2;
double pMaxSpeed = 0.15;

double looptime = 0.0001;
double time; 
bool first = true;
unsigned int ct;
bool finished = false;

void setup(){
    Serial.begin(9600);
    left.servo.attach(10);
    right.servo.attach(11);

    left.sensorPin = 5;
    right.sensorPin = 7;

    pinMode(left.sensorPin, INPUT);
    pinMode(right.sensorPin, INPUT);



    
    left.currentSpeed = 0;
    left.targetSpeed = 0;
    right.currentSpeed = 0;
    right.targetSpeed = 0;
}

void drive(float leftSpeed, float rightSpeed){
    left.servo.writeMicroseconds(1496.8+ 586.71*leftSpeed);
    right.servo.writeMicroseconds(1498-592.32*rightSpeed);

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
    left.sensorTrigered = false; 
    right.sensorTrigered = false;


    left.targetSpeed = 0.15;
    right.targetSpeed = 0.15;

            

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
            Serial.print("speed reached");
            }
        }
        else{
            side.currentSpeed -= speedDelta; 
            if(side.targetSpeed > side.currentSpeed){
                side.currentSpeed = side.targetSpeed;
            }
        }
    }


    drive(left.currentSpeed, right.currentSpeed);
    if(ct%1000 == 0){
        Serial.print("left: ");
        Serial.println(left.currentSpeed);
        Serial.print("right: ");
        Serial.println(right.currentSpeed);
    }
}