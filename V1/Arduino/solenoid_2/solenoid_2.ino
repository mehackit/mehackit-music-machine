/**
* Get a character between G-L and trigger a solenoid
* Change values in noteDuration array to change the 'lenght' 
**/


int pins[] = {8,9,10,11,12,13};
int pinCount = 6;
int noteDuration[] = {50, 50, 50, 50, 50, 50};
long endTimes[] = {0,0,0,0,0,0,0,0};

unsigned long time;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < pinCount; i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH);
  }
}

void loop() {
  while (Serial.available()) {
    processSerial();
  }
  
  time = millis();
  for (int i = 0; i < pinCount; i++) {
    if (time > endTimes[i]) {
      digitalWrite(pins[i], HIGH);   
    }
  }
}

void processSerial() {
  char input = Serial.read();
  int value = charToInt(input);
  if (0 <= value && value < 6) {
    digitalWrite(pins[value], LOW);
    endTimes[value] = millis() + noteDuration[value];
  } else {
    Serial.write(input);
  }   
}

int charToInt(char c) {
  return (int)c - 71; // https://www.arduino.cc/en/Reference/ASCIIchart
}
