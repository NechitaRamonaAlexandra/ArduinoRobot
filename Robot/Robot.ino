#include <Servo.h>
Servo srv;

// Pins of motor 1
#define mpin00 5
#define mpin01 6
// Pins of motor 2
#define mpin10 3
#define mpin11 11

int trigPin = 10;
int echoPin = 9;

int currentAngle = 180;
int rotationDirection = 1;
int completeScan = 0;

int laserPin = 13;
int led = 12;

long duration;
int distance;

void setup() {
  //Ultrasonic sensor setup
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  
  pinMode(laserPin, OUTPUT);

  //motors' pins setup(no movement at first)
  digitalWrite(mpin00, 0);
  digitalWrite(mpin01, 0);
  digitalWrite(mpin10, 0);
  digitalWrite(mpin11, 0);
  pinMode (mpin00, OUTPUT);
  pinMode (mpin01, OUTPUT);
  pinMode (mpin10, OUTPUT);
  pinMode (mpin11, OUTPUT);
}

//function that uses the data from the ultrasonic sensor and determines if there is an obstacle or not
//the sensor's range was maximum 30 cm
boolean isSomethingThere(int dist) {
  if(dist > 0 && dist < 30) {
    digitalWrite(led,LOW);
    delay(500);
    digitalWrite(led,HIGH);
    return true;
  }
  digitalWrite(laserPin,LOW);
  return false;
}

//function for initializing server motor(run just once)
void initServo(int pin) {
  srv.attach(pin);
  srv.write(currentAngle);
  delay(100);
  srv.detach();
}

//Using the mototrs for forward movement at configurable speed
void forwardMotor(int m1, int m2, int speed) {
  digitalWrite(m2,0);
  analogWrite(m1,speed);
}

//function for moving servo 20 degrees left/right depending on current direction
void playWithServo(int pin,int &currentAngle){
    srv.attach(pin);
    if(currentAngle == 180) {
      currentAngle -= 20;
      delay(200);
      srv.write(currentAngle);
      rotationDirection = 1;
    }
    else if(currentAngle == 0) {
      rotationDirection = 0;
      currentAngle += 20;
      completeScan = 1;
      delay(200);
      srv.write(currentAngle);
    }
    else if(rotationDirection == 0) {
      currentAngle += 20;
      delay(200);
      srv.write(currentAngle);
    }
    else if(rotationDirection == 1){
      currentAngle -= 20;
      delay(200);
      srv.write(currentAngle);
    }
    srv.detach();
}

void loop() {
  digitalWrite(trigPin,LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  duration = pulseIn(echoPin,HIGH);

  distance =  duration*0.034/2;

  initServo(8);

  if(isSomethingThere(distance)){
    //start the laser
    digitalWrite(laserPin,HIGH);
    digitalWrite(led,HIGH);
  }
  else {
    playWithServo(8,currentAngle); 
  }

  if(completeScan == 1) {
    forwardMotor(mpin00,mpin01,128);
    forwardMotor(mpin10,mpin11,128);
    delay(1000);
    completeScan = 0;
  }
  
  //stop motor
  digitalWrite(mpin00, 0);
  digitalWrite(mpin01, 0);
  digitalWrite(mpin10, 0);
  digitalWrite(mpin11, 0);

}
