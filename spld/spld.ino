#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

#define CODE_NONE        0
#define CODE_SERVO_LEFT  1
#define CODE_SERVO_RIGHT 2
#define CODE_SERVO_STOP  3

#define CODE_MOTOR_BACKWARD  4
#define CODE_MOTOR_FORWARD   5
#define CODE_MOTOR_STOP      6

#define VRX_PIN1  A0 
#define VRY_PIN1  A1
#define VRX_PIN2  A2 
#define VRY_PIN2  A3  
 
#define LEFT_THRESHOLD  400
#define RIGHT_THRESHOLD 800
#define UP_THRESHOLD    400
#define DOWN_THRESHOLD  800

#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

#define SERVO_PIN1 9
#define SERVO_PIN2 10

#define RPWM 5
#define LPWM 6
#define REN 7 
#define LEN 8 

int xValue1 = 0 ; 
int yValue1 = 0 ; 
int xValue2 = 0 ; 
int yValue2 = 0 ; 

float pos = 90;
int direction = 1;
int pot;
int out1;
int out2;

int command = COMMAND_NO;
unsigned long message_code = CODE_NONE;

Servo servo1;
Servo servo2;

void setup()
{
  Serial.begin(9600); 
  Serial.println("Start");

  pinMode(RPWM,OUTPUT);
  pinMode(LPWM,OUTPUT);
  pinMode(LEN,OUTPUT);
  pinMode(REN,OUTPUT);

  digitalWrite(REN,HIGH);
  digitalWrite(LEN,HIGH);

  servo1.attach(SERVO_PIN1);
  servo2.attach(SERVO_PIN2);
}

void leftDirection()
{
  for(pos = 90; pos <= 180; pos += 1) 
  {
    servo1.write(pos);
    servo2.write(pos);
  }
}

void rightDirection()
{
  for(pos = 90; pos >= 0; pos -= 1)
  {
    servo1.write(pos);
    servo2.write(pos);
  }
}

void forwardDirection(int speed)
{
  Serial.println("MOTOR FORWARD");
  if(speed > 0){
    out1 = map(speed, 0, 1023, 0, 255);
    analogWrite(LPWM, out1);
    analogWrite(RPWM, 0);
  } else {
    stopMotor();
  }
}

void backwardDirection(int speed)
{
  Serial.println("MOTOR BACKWARD");
  if(speed > 0){
    out1 = map(speed, 0, 1023, 0, 255);
    analogWrite(RPWM, out1);
    analogWrite(LPWM, 0);
  } else {
    stopMotor();
  }
}

void stopMotor()
{
  Serial.println("MOTOR STOP");
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);
}

void stopServo()
{
  Serial.println("MOTOR STOP");
  servo1.write(pos);
  servo2.write(pos);
}

void loop()
{
  xValue1 = analogRead(VRX_PIN1);
  yValue1 = analogRead(VRY_PIN1);

  xValue2 = analogRead(VRX_PIN2);
  yValue2 = analogRead(VRY_PIN2);

  command = COMMAND_NO;
  
  if (xValue1 < LEFT_THRESHOLD) {

    Serial.println("COMMAND LEFT");
    leftDirection();

  } else if (xValue1 > RIGHT_THRESHOLD) {

    rightDirection();
    Serial.println("COMMAND RIGHT");

  } else {

    stopServo();
    Serial.println("NO COMMAND");

  }

  if (yValue2 < UP_THRESHOLD) {

    forwardDirection(yValue2);
    Serial.println("MOTOR FORWARD");

  } else if (yValue2 > DOWN_THRESHOLD) {

    backwardDirection(yValue2);
    Serial.println("MOTOR BACKWARD");

  } else {
    
    stopMotor();
    Serial.println("MOTOR STOP");

  }
}
