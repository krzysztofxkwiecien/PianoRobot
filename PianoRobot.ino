#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50
#define SERVO_COUNT 2

// Servo positions
#define LEFT  200
#define MIDDLE  325
#define RIGHT  450

// Config
#define NOTES_COUNT 4

// Mapping
#define G_NOTE 0
#define A_NOTE 1
#define B_NOTE 2
#define C_NOTE 3

int noteToServoID[NOTES_COUNT];
int noteToServoPosition[NOTES_COUNT];

// Song
int noteLength = 150;
int notes[] = {G_NOTE, A_NOTE, C_NOTE, A_NOTE, G_NOTE, B_NOTE, A_NOTE, A_NOTE};
int durations[] = {1, 1, 1, 1, 1, 1, 1, 1};
const int noteCount = 8;
int currentNote = 0;

void setup() {

  for(int note = 0; note < NOTES_COUNT; note++){
    noteToServoID[note] = (int)(note / 2);
    noteToServoPosition[note] = (note % 2 == 0) ? LEFT : RIGHT;
  }
  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  delay(2000);
}


void loop() {
//  for(int i = 0; i < SERVO_COUNT; i++){
//    turnServo(i, MIDDLE);
//  }

  playNote(notes[currentNote], durations[currentNote]);

  if(++currentNote == noteCount)
    currentNote = 0;
}

void playNote(int note, int duration){
  playSimpleNote(note);
  delay(noteLength * duration);
  turnServo(noteToServoID[note], MIDDLE);
}

void playSimpleNote(int note){
  turnServo(noteToServoID[note], noteToServoPosition[note]);
}

void turnServo(int servoID, int position){
  pwm.setPWM(servoID, 0, position);
}
