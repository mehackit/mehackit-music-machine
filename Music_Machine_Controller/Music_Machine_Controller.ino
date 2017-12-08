/**
 * Mehackit Music Machine Solenoid Controller
 * 
 * Solenoid controller is a custom ATMEGA board that takes in MIDI
 * and opens VIN current through a corresponding transistor i.e. triggers
 * a salonoid based on the incoming MIDI.
 * 
 * A 3 bit DIP-switch defines the MIDI notes. 0 is notes 0-7, 1 is 8-15 etc.
 * When a MIDI note on is received, a solenoid is triggered for 50ms. All midi
 * is passed to MIDI through.
 * 
 * Otso Sorvettula / Mehackit 2017
 */

#include <MIDI.h>  // Add Midi Library
#include <Servo.h>

#define ledPin 13
#define S1 2
#define S2 3
#define S3 4
#define S4 5
#define S5 6
#define S6 7
#define S7 8
#define ammountOfSolenoids 7
#define ammountOfNotes 9
#define SERVO_1_PIN 9
#define SERVO_2_PIN 10
#define SERVO_1_NOTE 7
#define SERVO_2_NOTE 8

int solenoids[ammountOfSolenoids] = {S1, S2, S3, S4, S5, S6, S7};
int dipPins[2] = {11,12};
int scale = 0;
long notesOff[ammountOfSolenoids];
unsigned long noteLength = 30;
unsigned long now = 0;
boolean servo1AtOrigin = true;
boolean servo2AtOrigin = true;
Servo servo1;
Servo servo2;

//Create an instance of the library with default name, serial port and settings
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S4, OUTPUT);
  pinMode(S5, OUTPUT);
  pinMode(S6, OUTPUT);
  pinMode(S7, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  for (int i = 0; i < 3; ++i) {
    pinMode(dipPins[i], INPUT_PULLUP);
  }

  for (int i = 0; i < ammountOfSolenoids; i++) {
    notesOff[i] = 0;
  }

  scale = getDipPinsValToInt();

  MIDI.begin(MIDI_CHANNEL_OMNI); //Listen to all MIDI channels
  MIDI.setHandleNoteOn(MyHandleNoteOn);
  MIDI.setHandleNoteOff(MyHandleNoteOff);

  servo1.attach(SERVO_1_PIN);
  servo2.attach(SERVO_2_PIN);
  //servo1.write(0);
  //servo2.write(0);
}

void loop() { // Main loop
  MIDI.read(); // Continuously check if Midi data has been received.
  now = millis();
  for (int i = 0; i < ammountOfSolenoids; i++) {
    if (notesOff[i] < now) {
      digitalWrite(solenoids[i], LOW);
    }
  }
}

int getDipPinsValToInt() {
  byte result = 0;
  for (int i = 0; i < 2; ++i) {
    bool switchOn = digitalRead(dipPins[i]);
    if (switchOn) {
      result = result | (1 << i);
    }
  }
  return (int) result;
}

// MyHandleNoteON is the function that will be called by the Midi Library
// when a MIDI NOTE ON message is received.
// It will be passed bytes for Channel, Pitch, and Velocity
void MyHandleNoteOn(byte channel, byte pitch, byte velocity) {
  digitalWrite(ledPin, HIGH);
  int note = pitch - ammountOfNotes * scale;
  if (note >= 0 && note < ammountOfNotes) {
    if (note < ammountOfSolenoids) {
      digitalWrite(solenoids[note], HIGH);
      notesOff[note] = millis() + noteLength;  
    } else {
      turnServo(note);
    }
  }
}



void turnServo(int note) {
  if (note == SERVO_1_NOTE) {
    if (servo1AtOrigin) {
      servo1.write(150);
    } else {
      servo1.write(30);
    }
    servo1AtOrigin = !servo1AtOrigin;
  } else if (note == SERVO_2_NOTE) {
    if (servo2AtOrigin) {
      servo2.write(150);
    } else {
      servo2.write(30);
    }
    servo2AtOrigin = !servo2AtOrigin;
  }
}

// MyHandleNoteOFF is the function that will be called by the Midi Library
// when a MIDI NOTE OFF message is received.
// * A NOTE ON message with Velocity = 0 will be treated as a NOTE OFF message *
// It will be passed bytes for Channel, Pitch, and Velocity
void MyHandleNoteOff(byte channel, byte pitch, byte velocity) {
  digitalWrite(ledPin, LOW);
}

