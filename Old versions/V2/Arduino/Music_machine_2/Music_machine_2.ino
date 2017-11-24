#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11); // RX, TX

const char messages[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};

const int buttonPins[] = {A0, A1, A5, A3};
unsigned long nextButtonPressTime[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
unsigned long time = 0;

int tempoPin = A4;
int tempo = 0;
int lastTempo = 0;

int resetButton = 4;
unsigned long nextReset = 0;

int clickTime = 100;
unsigned long lastClick = 0;

const int duration = 200;

int latchPin = 8;
int clockPin = 7;
int dataPin2 = 6;
int dataPin = 5;

byte dataArray[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
int dataIndex = 15;


void setup() {
  Serial.begin(9600);
  Serial.begin(9600);
  softSerial.begin(9600);

  pinMode(resetButton, INPUT_PULLUP);
  pinMode(latchPin, OUTPUT);
}

void loop() {
  time = millis();
  for (int i = 0; i < 4; i++) {
    readButtons(analogRead(buttonPins[i]), i); 
  }
  
  while (Serial.available()) {
    char value = Serial.read();
    if (value == 'X') {
      nextLedOn();
      lastClick = time;
    } else {
      softSerial.write(value);
    }
  }
  
  //if (time - lastClick > clickTime) {
    //currentLedOff();
  //}
  
  if (digitalRead(resetButton) == LOW && nextReset < time) {
    Serial.print('W');
    nextReset = time + duration;
    dataIndex = 15;
  }
  
  tempo = (int) map(analogRead(tempoPin), 0, 1023, 50, 220);
  if (tempo != lastTempo) {
    Serial.print(tempo);
    lastTempo = tempo;
  } 
}

void nextLedOn() {
  if (dataIndex < 15) {
    dataIndex++;
  } else {
    dataIndex = 0;
  }
  
  if (dataIndex < 8) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin2, clockPin, 0);
    shiftOut(dataPin, clockPin, dataArray[dataIndex]);
    digitalWrite(latchPin, 1);
  } else {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin2, clockPin, dataArray[dataIndex - 8]);
    digitalWrite(latchPin, 1);
  }
}

void currentLedOff() {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin2, clockPin, 0);
  digitalWrite(latchPin, 1);
}

void readButtons(int value, int r) {
  if (value > 10) {
    if (value < 662 && value > 652 && nextButtonPressTime[r][3] < time) {
      //buttons[r][3] = 1;
      //nextButtonPressTime[r][3] = time + duration;
      sendMessage(r, 3);
    } else if (value < 593 && value > 583 && nextButtonPressTime[r][2] < time) {
      //buttons[r][2] = 1;
      //nextButtonPressTime[r][2] = time + duration;
      sendMessage(r, 2);
    } else if (value < 489 && value > 479 && nextButtonPressTime[r][1] < time) {
      //buttons[r][1] = 1;
      //nextButtonPressTime[r][1] = time + duration;
      sendMessage(r, 1);
    } else if (value < 321 && value > 311 && nextButtonPressTime[r][0] < time){
      //buttons[r][0] = 1;
      //nextButtonPressTime[r][0] = time + duration;
      sendMessage(r, 0);
    }
  }
}

void sendMessage(int r, int i) {
  nextButtonPressTime[r][i] = time + duration;
  Serial.print(messages[r*4+i]);
  softSerial.write(messages[r*4+i]);
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

//internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

//clear everything out just in case to
//prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}



