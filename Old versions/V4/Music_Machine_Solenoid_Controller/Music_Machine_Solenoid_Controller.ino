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
#define ledPin 13
#define S1 12
#define S2 11
#define S3 10
#define S4 9
#define S5 8
#define S6 7
#define S7 6
#define S8 5
#define ammountOfSolenoids 8

int solenoids[ammountOfSolenoids] = {S1, S2, S3, S4, S5, S6, S7, S8};
int dipPins[3] = {4,3,2};
int scale = 0;
long notesOff[ammountOfSolenoids];
unsigned long noteLength = 30;
unsigned long now = 0;

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
  pinMode(S8, OUTPUT);
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
  for (int i = 0; i < 3; ++i) {
    bool switchOn = !digitalRead(dipPins[i]); //pullup reverses the logic
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
  int solenoid = pitch - ammountOfSolenoids * scale;
  if (solenoid >= 0 && solenoid < ammountOfSolenoids) {
    digitalWrite(solenoids[solenoid], HIGH);
    notesOff[solenoid] = millis() + noteLength;
  }
}

// MyHandleNoteOFF is the function that will be called by the Midi Library
// when a MIDI NOTE OFF message is received.
// * A NOTE ON message with Velocity = 0 will be treated as a NOTE OFF message *
// It will be passed bytes for Channel, Pitch, and Velocity
void MyHandleNoteOff(byte channel, byte pitch, byte velocity) {
  digitalWrite(ledPin, LOW);
}

