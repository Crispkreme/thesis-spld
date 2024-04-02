#include <Servo.h>

#define RPWM 9
#define LPWM 10
#define PWM 11

Servo servo1;
Servo servo2;

int x_key = A1;
int y_key = A0;
int x_pos;
int y_pos;
int servo1_pin = 6;
int servo2_pin = 7;
int initial_position = 90;

void setup() {
  Serial.begin(9600);
  
  servo1.attach(servo1_pin);
  servo2.attach(servo2_pin);

  servo1.write(initial_position);
  servo2.write(initial_position);

  pinMode(x_key, INPUT);
  pinMode(y_key, INPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(PWM, OUTPUT);
}

void loop() {
  x_pos = analogRead(x_key);
  y_pos = analogRead(y_key);

  int servoPos = map(x_pos, 0, 1023, 0, 180);
  int motorSpeed = map(y_pos, 0, 1023, 0, 255);

  servo1.write(servoPos);
  servo2.write(servoPos);

  if (y_pos < 512) {
    motor_cw(motorSpeed);
  } else if (y_pos > 512) {
    motor_ccw(motorSpeed);
  } else {
    motor_stop();
  }
}

void motor_cw(int speed){
  digitalWrite(LPWM, LOW);
  digitalWrite(RPWM, HIGH);
  analogWrite(PWM, speed);
  Serial.println("Motor runs CW");
}

void motor_ccw(int speed){
  digitalWrite(LPWM, HIGH);
  digitalWrite(RPWM, LOW);
  analogWrite(PWM, speed);
  Serial.println("Motor runs CCW");
}

void motor_stop(){
  digitalWrite(LPWM, LOW);
  digitalWrite(RPWM, LOW);
  analogWrite(PWM, 0);
  Serial.println("Motor stopped");
}
