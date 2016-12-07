#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11); // RX, TX

const int amountOfButtons = 5;
unsigned long time;
unsigned long nextPressTime[amountOfButtons];
unsigned long duration = 200;

char button1Messages[] = {'A', 'B'};
char button2Messages[] = {'G', 'H'};
char analogButton1Messages[] = {'C'};
char analogButton2Messages[] = {'D'};
char analogButton3Messages[] = {'E'};
char analogButton4Messages[] = {'F'};

int click = 5;
int clickTime = 100;
int lastClick = 0;

int tempoPin = A2;
int tempo = 0;
int lastTempo = 0;

int button1 = 2;
int button2 = 3;
int analogButton1 = A1;
int analogButton2 = A0;
int resetButton = 4;

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(click, OUTPUT);
  
  Serial.begin(9600);
  softSerial.begin(9600);

  for(int i = 0; i < amountOfButtons; i++) {
    nextPressTime[i] = 0;
  }
}

void loop() {
  time = millis();
  
  if (digitalRead(button1) == LOW && nextPressTime[0] < time) {
    sendCtrlMessage(button1Messages[random(sizeof(button1Messages))], 0);
  }
  
  if (digitalRead(button2) == LOW && nextPressTime[1] < time) {
    sendCtrlMessage(button2Messages[random(sizeof(button2Messages))], 1);
  }
  
  if (nextPressTime[2] < time) {
    int value = analogRead(analogButton1);
    if (value < 10) {
      sendCtrlMessage(analogButton1Messages[random(sizeof(analogButton1Messages))], 2);
    } else if (value > 1013) {
      sendCtrlMessage(analogButton2Messages[random(sizeof(analogButton2Messages))], 2);
    }
  }
  
   if (nextPressTime[3] < time) {
    int value2 = analogRead(analogButton2);
    if (value2 < 10) {
      sendCtrlMessage(analogButton3Messages[random(sizeof(analogButton3Messages))], 3);
    } else if (value2 > 1013) {
      sendCtrlMessage(analogButton4Messages[random(sizeof(analogButton4Messages))], 3);
    }
  }
  
  if (digitalRead(resetButton) == LOW && nextPressTime[4] < time) {
    Serial.print('W');
    nextPressTime[4] = time + duration;
  }

  while (Serial.available()) {
    char value = Serial.read();
    if (value == 'X') {
      digitalWrite(click, HIGH);
      lastClick = time;
    }
    softSerial.write(value);
  }
  
  if (time - lastClick > clickTime) {
    digitalWrite(click, LOW);
  }
  
  tempo = (int) map(analogRead(tempoPin), 0, 1023, 0, 9);
  if (tempo != lastTempo) {
    Serial.print(tempo);
    lastTempo = tempo;
  }  
}

void sendCtrlMessage(char x, int buttonIndex) {
  Serial.print(x);
  softSerial.write(x);
  nextPressTime[buttonIndex] = time + duration;
}
