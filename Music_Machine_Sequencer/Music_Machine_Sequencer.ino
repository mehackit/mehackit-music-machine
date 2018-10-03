/**
   Mehackit Music Machine Sequencer

   Music Machine Sequencer is a Teensy based sequencer
   with 8 knobs that choose between predefined sequences.



   Otso Sorvettula / Mehackit 2017
*/

#include <AnalogSmooth.h>
#include <MIDI.h>
#include <Bounce.h>
MIDI_CREATE_DEFAULT_INSTANCE();

boolean useFirstNote[8] = {1, 1, 1, 1, 1, 1, 1, 1};

int midiNotes[8][2] = {
  {0, 0},
  {1, 1},
  {2, 2},
  {3, 3},
  {4, 4},
  {5, 5},
  {6, 6},
  {7, 7},
};

// Kick
const bool sequence1[10][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
  {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
};

// Snare
const bool sequence2[10][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
};

// Hihat
const bool sequence3[10][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1},
  {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
  {1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
};

// Perc
const bool sequence4[10][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1},
  {0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
};


const bool sequence5[10][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
  {0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
};

const bool sequence6[10][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
  {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
};
const bool sequence7[10][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
  {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
};
const bool sequence8[10][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
  {1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
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
int channel = 1;

void setup() {
  Serial.begin(9600);
  MIDI.begin();
  interval = bpmToInterval(tempo);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (micros() > nextPosition) {
    if (position % 2 == 0) {
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

      //Send random midi note
      int i = random(8);
      MIDI.sendNoteOn(midiNotes[i][0], 127, channel);
      MIDI.sendNoteOn(midiNotes[i][1], 127, channel);
    } else {
      buttonPressed = false;
      Serial.println("Button released");
    }
  }

  if (buttonPressed) {
    newSwing = asTempo.smooth(analogRead(tempoPin));
    if (abs(newSwing - lastSwing) > 3) {
      lastSwing = newSwing;
      swing = (map(newSwing, 0, 1024, 0, 50)) / 100.0;
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
  unsigned long interval = 60 * 1000 * 1000 / bpm / 4;
  return interval;
}

void advance() {
  // Send midi off for all notes
  for (int i = 0; i < 8; ++i) {
    MIDI.sendNoteOff(midiNotes[i][0], 127, channel);
    MIDI.sendNoteOff(midiNotes[i][1], 127, channel);
  }

  // Send midi on corresponding to the sequences
  for (int i = 0; sequences[i]; i++) {
    int seqNumber = map(analogRead(sequencePins[i]), 0, 1024, 0, 9);
    Serial.println(seqNumber);
    if (sequences[i][seqNumber][position]) {
      if (useFirstNote[i]) {
        //if(position % 2) {
        MIDI.sendNoteOn(midiNotes[i][0], 127, channel);
      } else {
        MIDI.sendNoteOn(midiNotes[i][1], 127, channel);
      }
      useFirstNote[i] = !useFirstNote[i];
    }
  }

  // Advance
  position = (position + 1) % SEQ_LENGTH;
}


