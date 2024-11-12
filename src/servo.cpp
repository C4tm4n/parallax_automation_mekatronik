#include <arduino.h> 
#include <Servo.h> 
void updateSpeeds();

void readSensors();
struct motor{

    Servo servo;
    double currentSpeed;
    double targetSpeed;
    int sensorPin;
    int sensorTrigered;
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
bool drivebackward = false;
bool driveLeft = false;
bool driveRight = false;
bool stop = false;
double lastStop;
int turnTime;

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
    left.servo.writeMicroseconds(1496.8- 586.71*leftSpeed);
    right.servo.writeMicroseconds(1498+592.32*rightSpeed);

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
    if(stop){
        left.targetSpeed = 0;
        right.targetSpeed = 0;
        if(left.currentSpeed == 0 && right.currentSpeed == 0){
            stop = false;
            lastStop = time;
        }
        
    }
    else if(drivebackward){
        left.targetSpeed = -0.15;
        right.targetSpeed = -0.15;

    }
    else if(driveLeft){
        left.targetSpeed = -0.15;
        right.targetSpeed = 0.15;
        if(lastStop + turnTime <time){
            driveLeft = false;
        }
    }
    else if(driveRight){
        left.targetSpeed = 0.15;
        right.targetSpeed = -0.15;
        if(lastStop + turnTime <time){
            driveRight = false;
        }
    }
    else{
        left.targetSpeed = 0.15;
        right.targetSpeed = 0.15;

    }

    if(left.sensorTrigered == 1 && right.sensorTrigered == 1){
        if(drivebackward){
            drivebackward = false;
            stop = true;
        }


    }
    else if(left.sensorTrigered == 1) //if right sensor trigered
    {
        drivebackward = true;
        driveLeft = true;
        stop = true;
        turnTime = 0.42;
        if(ct%1000 == 0)
        {
            Serial.println("turning left");
        }
    }
    else if(right.sensorTrigered == 1) //if left sensor trigered
    {
        drivebackward = true;
        driveRight = true;
        stop = true;
        turnTime = 0.42;
        if(ct%1000 == 0)
        {
            Serial.println("turning right");
        }
    }
    else
    {
        drivebackward = true;
        driveLeft = true;
        stop = true;
        turnTime = 0.69;
        if(ct%1000 == 0)
        {
            Serial.println("driving backward");
        }
    }

            

    updateSpeeds();
}

void readSensors(){
    left.sensorTrigered = digitalRead(left.sensorPin);
    right.sensorTrigered = digitalRead(right.sensorPin);
    if(ct%1000 == 0)
    {
        Serial.print("sensors r + l:");
        Serial.print(right.sensorTrigered);
        Serial.println(left.sensorTrigered);
    }

}



float calculateSpeedDelta(struct motor servo){
    double currentAcceleration;
    //currentAcceleration = acceleration*(tMaxSpeed - servo.currentSpeed);
    currentAcceleration = 0.3;
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