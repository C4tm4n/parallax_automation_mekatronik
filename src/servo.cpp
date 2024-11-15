#include <arduino.h> 
#include <constants.h>
#include <Servo.h> 
#include <cppQueue.h>
void updateSpeeds();
void readSensors();
void calculatePosition(int movement);
void debug();
void debug(String msg, int freq);
void estimateMovement();
void evaluateSensorReadings();
void performAction();
struct motor{

    Servo servo;
    double currentSpeed;
    double targetSpeed;
    int sensorPin;
    int sensorTrigered;
};
struct motor left;
struct motor right;


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
enum sensorReading{
    BOTH,
    CLEAR,
    SLEFT,
    SRIGHT
}currentReading;


enum turns {
    RIGHT,
    LEFT,
    NONE,
    UTURN,
    BACKWARD,
    STOP,
    EVALUATE
};
turns turn;
turns currentAction;
turns nextAction;

cppQueue actions(sizeof(turn),3, FIFO);


int xPos = 0;
int yPos = 0;
int rotation = 0;


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
        looptime = micros()/1000000.0 -time;
        time = micros()/1000000.0;
    
    }
    evaluateSensorReadings();

            

    updateSpeeds();
    debug();
    estimateMovement();
}
void performAction(){

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
        debug("driving backward",1000);

    }
    else if(turn == LEFT){
        left.targetSpeed = -0.15;
        right.targetSpeed = 0.15;
        debug("setting left turn",1000);
        if(lastStop + turnTime <time){
            turn = NONE;
        }
    }
    else if(turn == RIGHT){
        debug("setting right turn",1000);
        left.targetSpeed = 0.15;
        right.targetSpeed = -0.15;
        if(lastStop + turnTime <time){
            turn = NONE;
        }
    }
    else{
        left.targetSpeed = 0.15;
        right.targetSpeed = 0.15;
    }

}

void evaluateSensorReadings(){
    if(left.sensorTrigered == 1 && right.sensorTrigered == 1){ //no sensor triggered 
        currentReading = CLEAR;
        if(drivebackward){
            drivebackward = false;
            stop = true;
            debug("stopping driving backward", 1);
        }
    }
    else if(left.sensorTrigered == 1) //if right sensor trigered
    {
        currentReading = SRIGHT;
        drivebackward = true;
        turn = LEFT;
        stop = true;
        turnTime = 2.42;
        obstacle();
        if(nextAction == NONE ){
            currentAction = BACKWARD;
            nextAction = LEFT;
        }
    }
    else if(right.sensorTrigered == 1) //if left sensor trigered
    {
        currentReading = SLEFT;
        drivebackward = true;
        turn = RIGHT;
        stop = true;
        turnTime = 2.42;
        obstacle();
        if(nextAction == NONE ){
            currentAction = BACKWARD;
            nextAction = RIGHT;
        }
    }
    else //both sensors triggered
    {

        currentReading = BOTH;
        drivebackward = true;
        turn = LEFT;
        stop = true;
        turnTime = 10.69;
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
    

    int relativeRotation = diff/WheelBase;
    rotation += relativeRotation;
    calculatePosition(movement);
}
void calculatePosition(int movement){
    xPos += movement* asin(rotation);
    yPos += movement* acos(rotation);
}


void mapping(){

}

void obstacle(){
    int obstacleX = xPos + sensorOffset/cos(rotation);


}

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
        Serial.println(turn);
        Serial.print("looptime: ");
        Serial.println(looptime,6);
        Serial.println(time,6);
        Serial.println(lastStop,6);
        Serial.println(turn);

    }

}
void debug(String msg, int freq){
    if(ct%freq == 0){
        Serial.println(msg);
    }
}
void readSensors(){
    left.sensorTrigered = digitalRead(left.sensorPin);
    right.sensorTrigered = digitalRead(right.sensorPin);
}



float calculateSpeedDelta(struct motor servo){
    double currentAcceleration;
    //currentAcceleration = acceleration*(tMaxSpeed - servo.currentSpeed);
    if(abs(left.targetSpeed)>abs(left.currentSpeed)){
        currentAcceleration = acceleration;
    }
    else{
        currentAcceleration = breakAcceleration;
    }
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
        }
    }
    else{
        left.currentSpeed -= speedDelta; 
        if(left.targetSpeed > left.currentSpeed){
            left.currentSpeed = left.targetSpeed;
            }
    }


    speedDelta = calculateSpeedDelta(right);
    if(right.targetSpeed > right.currentSpeed)
    {

        right.currentSpeed += speedDelta; 
        if(right.targetSpeed<right.currentSpeed)
        { // if the new speed goes above the target speed
        right.currentSpeed = right.targetSpeed; 
        }
    }
    else{
        right.currentSpeed -= speedDelta; 
        if(right.targetSpeed > right.currentSpeed){
            right.currentSpeed = right.targetSpeed;
            }
    }

    drive(left.currentSpeed, right.currentSpeed);
}