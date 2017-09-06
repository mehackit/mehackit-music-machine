/**
   Mehackit Music Machine Sequencer

   Music Machine Sequencer is a Teensy based sequencer
   with 8 knobs that choose between predefined sequences.

   Otso Sorvettula / Mehackit 2017
*/

#include <AnalogSmooth.h>
#include <MIDI.h>
#include <Bounce.h>

int midiNotes[8][2] = {
  {0, 8},
  {1, 9},
  {2, 10},
  {3, 11},
  {4, 12},
  {5, 13},
  {6, 14},
  {7, 15},
};

bool sequence1[10][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

};
const bool sequence2[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
const bool sequence3[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
const bool sequence4[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
const bool sequence5[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
const bool sequence6[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
const bool sequence7[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
const bool sequence8[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

const int SEQ_LENGTH = 16;
const bool (*sequences[])[16] {
  sequence1,
  sequence2,
  sequence3,
  sequence4,
  sequence5,
  sequence6,
  sequence7,
  sequence8,
  NULL
};

const int tempoPin = A8;
const int pin1 = A0;
const int pin2 = A1;
const int pin3 = A2;
const int pin4 = A3;
const int pin5 = A4;
const int pin6 = A5;
const int pin7 = A6;
const int pin8 = A7;
const int sequencePins[8] = {pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8};

const int buttonPin = 2;
Bounce pushbutton = Bounce(buttonPin, 10);
byte buttonPressed = true;
float swing = 0; //Swing in percentage
float lastSwing = 0;
float newSwing = 0;

AnalogSmooth asTempo = AnalogSmooth(100);
int tempo = 120;
int newTempo = 120;

long interval;
unsigned long nextPosition = 0;
int position = 0;
int note = 0;
int channel = 1;

void setup() {
  Serial.begin(9600);
  MIDI.begin();
  interval = bpmToInterval(tempo);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (micros() > nextPosition) {
    if (note % 2 == 0) {
      nextPosition = micros() + (interval * (1 - swing));
    } else {
      nextPosition = micros() + (interval * (1 + swing));
    }
    advance();
  }

  if (pushbutton.update()) {
    if (pushbutton.fallingEdge()) {
      lastSwing = asTempo.smooth(analogRead(tempoPin));
      buttonPressed = true;
      Serial.print("Button pressed: ");
      Serial.println(lastSwing);
    } else {
      buttonPressed = false;
      Serial.println("Button released");
    }
  }
  
  if (buttonPressed) {
    newSwing = asTempo.smooth(analogRead(tempoPin));
    if (abs(newSwing - lastSwing) > 3) {
     lastSwing = newSwing;
     swing = (map(newSwing, 0, 1024, 0, 250)) / 1000.0;
     Serial.println(swing);
    }
  } else {
    newTempo = asTempo.smooth(analogRead(tempoPin));
    newTempo = map(newTempo, 0, 1024, 40, 240);
    if (abs(newTempo - tempo) > 1) {
      tempo = newTempo;
      Serial.println(tempo);
      interval = bpmToInterval(tempo);
    }
  }
}

unsigned long bpmToInterval(int bpm) {
  unsigned long interval = 60000000 / (bpm * (SEQ_LENGTH / 2));
  return interval;
}

void advance() {
  if (position % 2 == 0) {
    for (int i = 0; sequences[i]; i++) {
      int seqNumber = map(analogRead(sequencePins[i]), 0, 1024, 0, 8);
      //Serial.print("Knob ");
      //Serial.print(i);
      //Serial.print(" = ");
      //Serial.print(seqNumber);
      //Serial.print("\t");
      if (sequences[i][seqNumber][note]) {
        MIDI.sendNoteOn(midiNotes[i][0], 127, channel);
        MIDI.sendNoteOn(midiNotes[i][1], 127, channel);
      }
    }
    //Serial.println();
  } else {
    for (int i = 0; i < 8; ++i) {
      MIDI.sendNoteOff(midiNotes[i][0], 127, channel);
      MIDI.sendNoteOff(midiNotes[i][1], 127, channel);
    }
    note = (note + 1) % SEQ_LENGTH;
  }
  position = (position + 1) % (SEQ_LENGTH * 2);
}


