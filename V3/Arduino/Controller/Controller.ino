#include <AnalogSmooth.h>
#include <Bounce.h>

const int analogInPin0 = A0;
const int analogInPin1 = A1;
const int analogInPin2 = A4;
const int analogInPin3 = A2;
const int analogInPin4 = A3;

const int button1Pin = 11;
const int button2Pin = 12;
Bounce button1 = Bounce(button1Pin, 10);
Bounce button2 = Bounce(button2Pin, 10);

const int kickCtlNum = 0;
const int snCtlNum = 1;
const int hhCtlNum = 2;
const int percCtlNum = 3;
const int bpmCtlNum = 4;
const int button1CtlNum = 5;
const int button2CtlNum = 6;
const int channel = 0;

int newSensorValue0 = 0;
int newSensorValue1 = 0;
int newSensorValue2 = 0;
int newSensorValue3 = 0;
int newSensorValue4 = 0;

int oldSensorValue0 = 0;
int oldSensorValue1 = 0;
int oldSensorValue2 = 0;
int oldSensorValue3 = 0;
int oldSensorValue4 = 0;

AnalogSmooth as0 = AnalogSmooth(10);
AnalogSmooth as1 = AnalogSmooth(10);
AnalogSmooth as2 = AnalogSmooth(10);
AnalogSmooth as3 = AnalogSmooth(10);
AnalogSmooth as4 = AnalogSmooth(50);

void setup() {
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
}

void loop() {
  button1.update();
  button2.update();

  newSensorValue0 = as0.smooth(analogRead(analogInPin0));
  newSensorValue0 = map(newSensorValue0, 0, 1024, 10, 0);

  newSensorValue1 = as1.smooth(analogRead(analogInPin1));
  newSensorValue1 = map(newSensorValue1, 0, 1024, 10, 0);

  newSensorValue2 = as2.smooth(analogRead(analogInPin2));
  newSensorValue2 = map(newSensorValue2, 0, 1024, 10, 0);

  newSensorValue3 = as3.smooth(analogRead(analogInPin3));
  newSensorValue3 = map(newSensorValue3, 0, 1024, 10, 0);

  newSensorValue4 = as4.smooth(analogRead(analogInPin4));
  newSensorValue4 = map(newSensorValue4, 0, 1024, 127, 0);

  //check changes
  if(newSensorValue0 != oldSensorValue0){
    sendMIDI(kickCtlNum, newSensorValue0);
  }

  if(newSensorValue1 != oldSensorValue1){
    sendMIDI(snCtlNum, newSensorValue1);
  }

  if(newSensorValue2 != oldSensorValue2){
    sendMIDI(hhCtlNum, newSensorValue2);
  }

  if(newSensorValue3 != oldSensorValue3){
    sendMIDI(percCtlNum, newSensorValue3);
  }

  if(newSensorValue4 != oldSensorValue4){
    sendMIDI(bpmCtlNum, newSensorValue4);
  }

  if(button1.fallingEdge()){
    sendMIDI(button1CtlNum, 127);
  }

  if(button2.fallingEdge()){
    sendMIDI(button2CtlNum, 127);
  }

  oldSensorValue0 = newSensorValue0;
  oldSensorValue1 = newSensorValue1;
  oldSensorValue2 = newSensorValue2;
  oldSensorValue3 = newSensorValue3;
  oldSensorValue4 = newSensorValue4;

  while (usbMIDI.read()) {
  }
  delay(5);
}

void sendMIDI(int ctl, int value) {
  if(value > 0){
        value = value - 1;
    }
    usbMIDI.sendControlChange(ctl, value, channel);
}
