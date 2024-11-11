#include <arduino.h> 
#include <Servo.h> 
void updateSpeeds();

void readSensors();
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
    left.sensorTrigered = false; 
    right.sensorTrigered = false;
}

void drive(float leftSpeed, float rightSpeed){
    left.servo.writeMicroseconds(1496.8+ 586.71*leftSpeed);
    right.servo.writeMicroseconds(1498-592.32*rightSpeed);

}


void loop()
{
    readSensors();
    left.targetSpeed = 0.1;
    right.targetSpeed = 0.1;
    ct ++; 
    if(first){
        time = micros()/1000000.0;
        first = false;
    }
    else{
        looptime = micros()/100000.0 -time;
        time = micros()/1000000.0;
    
    }


    if(!left.sensorPin && !right.sensorPin){
        left.targetSpeed = 0.15;
        right.targetSpeed = 0.15;
    }

            

    updateSpeeds();
}

void readSensors(){
    if(digitalRead(left.sensorPin) == LOW){
        left.sensorTrigered = true;
    }
    else{
        left.sensorTrigered = false;
    }
    if(digitalRead(right.sensorPin) == LOW){
        right.sensorTrigered = true;
    }
    else{
        right.sensorTrigered = false;
    }
    if(ct%1000 == 0)
    {
        Serial.print("sensors r + l:");
        Serial.print(right.sensorTrigered);
        Serial.println(left.sensorTrigered);
    }

}



float calculateSpeedDelta(struct motor servo){
    double currentAcceleration;
    currentAcceleration = acceleration*(tMaxSpeed - servo.currentSpeed);
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
            if(ct%1000 == 0)
            {
            Serial.print("speed reached");
            }
            }
        }
        else{
            left.currentSpeed -= speedDelta; 
            if(left.targetSpeed > left.currentSpeed){
                left.currentSpeed = left.targetSpeed;
            }
    }

            if(ct%1000 == 0)
            {

                Serial.print("targetspeed: ");
                Serial.println(left.targetSpeed);
                Serial.print("speeddelta: ");
                Serial.println(speedDelta);
                Serial.print("currentspped: ");
                Serial.println(left.currentSpeed);
            }

        speedDelta = calculateSpeedDelta(right);
        if(right.targetSpeed > right.currentSpeed)
        {

            right.currentSpeed += speedDelta; 
            if(right.targetSpeed<right.currentSpeed)
            { // if the new speed goes above the target speed
            right.currentSpeed = right.targetSpeed; 
            if(ct%1000 == 0)
            {
            Serial.print("speed reached");
            }
            }
        }
        else{
            right.currentSpeed -= speedDelta; 
            if(right.targetSpeed > right.currentSpeed){
                right.currentSpeed = right.targetSpeed;
            }
    }

            if(ct%1000 == 0)
            {

                Serial.print("targetspeed: ");
                Serial.println(right.targetSpeed);
                Serial.print("speeddelta: ");
                Serial.println(speedDelta);
                Serial.print("currentspped: ");
                Serial.println(right.currentSpeed);
            }

    drive(left.currentSpeed, right.currentSpeed);
    if(ct%1000 == 0)
    {
        Serial.print("left: ");
        Serial.println(left.currentSpeed);
        Serial.print("right: ");
        Serial.println(right.currentSpeed);
    }
}