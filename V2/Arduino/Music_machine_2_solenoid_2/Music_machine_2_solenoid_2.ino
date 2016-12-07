/*
  Music Machine solenoid controller
  
  Receives a char via serial. If the char maches an element in the mesages array, 
  trigger a solenoid. Otherwise pass the char to serial.
 */
 
// Define accepted chars and corresponding pins
char messages[8] = {'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
int pins[8] = {6,7,8,9,10,11,12,13};

// Array for millisecond values to turn a solenoid off
unsigned long endTimes[8] = {0,0,0,0,0,0,0,0};

// Duration in milliseconds that the solenoid stays on
int duration = 50;

// Variable for saving time in milliseconds in the beginning of each loop
unsigned long time = 0;


// Start serial and set pins as output
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 8; i++) {
    pinMode(pins[i], OUTPUT);
  }
}


void loop() {
  time = millis(); //save current time
  
  //read serial
  while (Serial.available()) {
    processSerial();
  }
  
  //turn solenoids of if needed
  for (int i = 0; i < 8; i++) {
    if (time > endTimes[i]) {
      digitalWrite(pins[i], HIGH);
    }
  }
}


void processSerial() {
  boolean matchFound = false; //Does the serial input belong to this arduino?
  
  char input = Serial.read();
  for (int i = 0; i < 8; i++) {
    if (input == messages[i]) { //If input is in the messages array trigger a solenoid
      digitalWrite(pins[i], LOW);
      endTimes[i] = millis() + duration;
      matchFound = true;
    }
  }
  
  if (!matchFound) {
    Serial.write(input); //If input wasn't part of messages array, pass it on
  }  
}
