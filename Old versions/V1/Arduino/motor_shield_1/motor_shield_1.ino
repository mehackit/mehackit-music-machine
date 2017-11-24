/**
* Get a character between M-P and trigger a solenoid
* Change values in noteDuration array to change the 'lenght' 
**/

#include <AFMotor.h>
#include <Servo.h> 

int noteDuration[] = {100, 50, 100, 500};
unsigned long endTimes[] = {0,0,0,0};
unsigned long time;

// Two servos (not continuous rotation)
Servo servo1;  
Servo servo2;
int servoAngles[] = {0, 0};
Servo servos[] = {servo1, servo2};

// 3 DC motors
AF_DCMotor motor1(3);
AF_DCMotor motor2(4);

AF_DCMotor motors[] = {motor1, motor2};

void setup() {
  Serial.begin(9600);
  servo1.attach(9);
  servo2.attach(10);
  servo1.write(90);
  servo2.write(90);
  motor1.setSpeed(255);
  motor2.setSpeed(255);
}

void loop() {
  while (Serial.available()) {
    processSerial();
  }
  
  time = millis();
  for (int i = 0; i < 4; i++) {
    if (time > endTimes[i]) {
      if (i < 2) {
        motors[i].run(RELEASE);
      } else {
        servos[i-2].write(90);
      }
    }
  }
}

void processSerial() {
  char input = Serial.read();
  int value = charToInt(input);
  if (0 <= value && value < 2) {
    servoAngles[value] = 0; 
    servos[value].write(servoAngles[value]);
    endTimes[value+2] = millis() + noteDuration[value+2];
  } else if (2 <= value && value < 3) {
    value = value - 2;
    motors[value].run(FORWARD);
    endTimes[value] = millis() + noteDuration[value];
  } else {
    Serial.write(input);
  }   
}

int charToInt(char c) {
  return (int)c - 77; // https://www.arduino.cc/en/Reference/ASCIIchart
}

